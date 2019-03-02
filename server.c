#include <mqueue.h>
#include <string.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "request.h"

/* List definition for storing triplets */

struct node_t *head = NULL;

/* mutex and condition variables for the message copy */

pthread_mutex_t mutex_msg;
int message_not_copied = TRUE;
pthread_cond_t cond_msg;

int main(void)
{

  /* Server data */

  mqd_t q_server;        /* server queue */
  struct request msg;    /* message to receive */
  struct mq_attr q_attr; /* queue atributes */
  pthread_attr_t t_attr; /* thread atributes */

  /* Queue size */

  q_attr.mq_maxmsg = 20;
  q_attr.mq_msgsize = sizeof(struct request));

  /* List initialization */

  node_t *head = NULL;
  head = malloc(sizeof(node_t));

  request triplet = NULL;

  head->value = triplet;
  head->next = NULL;

  /* Initialization */

  q_server = mq_open(“SERVER”, O_CREAT | O_RDONLY, 0700, &attr);
  if (q_server == -1)
  {
    perror(”Can’t create server queue”);
    return 1;
  }

  pthread_mutex_init(&mutex_msg, NULL);
  pthread_cond_init(&cond_msg, NULL);
  pthread_attr_init(&attr);

  /* Thread atributes */
  pthread_attr_setdetachstate(&t_attr, PTHREAD_CREATE_DETACHED);

  while (TRUE)
  {
    mq_receive(q_server, &msg, sizeof(struct request), 0);
    pthread_create(&thid, &attr, process_message, &msg);

    /* Critical Section --> message copying */

    pthread_mutex_lock(&mutex_msg);

    while (message_not_copied)
    {
      pthread_cond_wait(&cond_msg, &mutex_msg);
    }

    message_not_copied = TRUE;
    pthread_mutex_unlock(&mutex_msg);
  }
}

void process_message(struct mensaje *msg)
{

  struct request *msg_local;
  struct mqd_t q_client;
  int op_code;

  /* Thread copies message to local message */
  pthread_mutex_lock(&mutex_msg);

  memcpy((char *)&msg_local, (char *)&msg, sizeof(struct request));

  /* wake up server */
  message_not_copied = FALSE; /* FALSE = 0 */

  int result;
  char *server_value1;
  float server_value2;

  pthread_cond_signal(&cond_msg);

  pthread_mutex_unlock(&mutex_msg);

  /* Execute client request and prepare reply */

  op_code = msg_local->operation_code;

  switch (op_code)
  {

  case 0: // Init

    /* Deletion of previous elements */

    while (head != null)
    {

      struct node_t *temp = head;
      head = head->next;

      free(temp);
    }

    head = malloc(sizeof(node_t)); // New instantiation of list

    break;

  case 1: // Set value

    if (server_exist(msg_local) == 0)
    {
      result = -1;
      perror("This key is already inserted!");

      // Return this value to client
    }

    struct node_t *dummy = head;

    while (dummy->next != NULL)
    {
      dummy = dummy->next;
    }

    dummy->next = malloc(size_t(node_t)); // New node creation

    strcpy(msg_local->key, dummy->next->data->key);       // Copy key from message to list
    strcpy(msg_local->value1, dummy->next->data->value1); // Copy value 1 from message to list
    dummy->next->data->value2 = msg_local->value2;        // Copy value 2 from message to list

    result = 0; // Value properly inserted

    break;

  case 2: // Get value

    if (server_exist(msg_local) == 0)
    { // Check if exists
      result = -1;
      perror("This key does not exist!");

      // Return this value to client
    }

    struct node_t *dummy = head;

    while (strcmp(dummy->data->key, msg_local->key) != 0)
    { // Find the node
      dummy = dummy->next;
    }

    strcpy(dummy->data->value1, server_value1); // Value 1 of the node to be returned in message
    server_value2 = dummy->data->value2;        // Value 2 of the node to be returned in message

    result = 0; // Value properly obtained

    break;

  case 3: // Modify value

    if (server_exist(msg_local) == 0)
    { // Check if exists
      result = -1;
      perror("This key does not exist!");

      // Return this value to client
    }

    struct node_t *dummy = head;

    while (strcmp(dummy->data->key, msg_local->key) != 0)
    { // Find the node
      dummy = dummy->next;
    }

    strcpy(msg_local->value1, dummy->data->value1); // Update value1 in list node
    dummy->data->value2 = msg_local->value2;        // Updaye value2 in list node

    result = 0;

    break;

  case 4: // Delete value by key

    if (server_exist(msg_local) == 0)
    {
      return -1; // Value does not exist
    }

    else if (strcmp(head->data->key, msg_local->key) == 0)
    { // Check first element

      struct node_t *temp = head;
      head = head->next;

      free(temp);
    }

    else
    {

      struct node_t *current = head;
      struct node_t *previous = NULL;

      while (current != null)
      {

        if (strcmp(head->data->key, msg_local->key) == 0)
        { // Key found

          previous->next = current->next;

          free(current);
        }

        previous = current;
        current = current->next;
      }
    }

    break;

  case 5: // Value exists

    result = server_exist(msg_local); // 0 if not, 1 if exists

    break;

  case 6: // Number of items

    struct node_t *dummy = head;
    int counter = 0;

    while (dummy != null)
    {

      counter++;
    }

    break;

  default:
    break;
  }

  /* Return result to client by sending it to queue */

  q_client = mq_open(msg_local.name, O_WRONLY);

  if (q_client == -1)
  {
    perror("Can’t open client queue");
  }

  else
  {
    mq_send(q_client, (char *)&result, sizeof(int), 0);
    mq_close(q_client);
  }

  pthread_exit(0);
}

int server_exist(struct request *msg_local)
{

  struct node_t *dummy = head;

  while (dummy != null)
  {

    if (strcmp(dummy->data->key, msg_local->key) == 0)
    {
      // Key exists
      return 1;
    }
    else
    {
      dummy = dummy->next; // Advance position in list
    }
  }

  return 0; // Key does not exist
}