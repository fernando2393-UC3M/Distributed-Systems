#include <mqueue.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "message.h"
#include "sll.c"

/* Definition of process message function */

void process_message(struct message *msg);

/* List definition for storing triplets */

#define SERVER_QUEUE "/SERVER_QUEUE"
#define CLIENT_QUEUE "/CLIENT_QUEUE"

#define TRUE 1
#define FALSE 0

/* mutex and condition variables for the message copy */

pthread_mutex_t mutex_msg;
int message_not_copied = TRUE;
pthread_cond_t cond_msg;

int main(void)
{

  /* Server data */

  mqd_t q_server;        /* Server queue */
  struct message msg;    /* Message to receive */
  struct mq_attr q_attr; /* Queue atributes */
  pthread_attr_t t_attr; /* Thread atributes */

  /* Queue size */

  q_attr.mq_maxmsg = 10;
  q_attr.mq_msgsize = sizeof(struct message);

  /* Initialization */

  q_server = mq_open(SERVER_QUEUE, O_CREAT | O_RDONLY, 0666, &q_attr);
  if (q_server == -1)
  {
    perror("Can’t create server queue");
    return -1;
  }
  else {
    printf("Server has been initiated \n");
  }

  pthread_mutex_init(&mutex_msg, NULL);
  pthread_cond_init(&cond_msg, NULL);
  pthread_attr_init(&t_attr);

  /* Thread atributes */
  pthread_attr_setdetachstate(&t_attr, PTHREAD_CREATE_DETACHED);

  pthread_t thid;

  while (TRUE)
  {

    if(mq_receive(q_server, (char *)&msg, sizeof(struct message), 0) == -1){
      perror("mq_receive error");
      return -1;
    }
    
    if(pthread_create(&thid, &t_attr, (void *)process_message, &msg) != 0){
      perror("Server thread creation error");
      return -1;
    }

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

void process_message(struct message *msg)
{

  struct message msg_local; // Local message
  mqd_t q_client;           // Client queue
  int op_code;              // Operation code sent by client

  /* Thread copies message to local message */
  pthread_mutex_lock(&mutex_msg);

  memcpy(&msg_local, msg, sizeof(struct message));

  /* Client request obtained --> start processing reply */
  printf("\n********************\n");
  printf("Received request from: %s\n", (char *)msg_local.q_name);
  printf("MESSAGE RECEIVED\n");
  printf("Operation code: %d\n", msg_local.operation_code);
  printf("Key: %s\n", msg_local.key);
  printf("Value1: %s\n", msg_local.value1);
  printf("Value2: %f\n", msg_local.value2);

  int result; // Result of the operation to be returned in the response

  /* Execute client message and prepare reply */

  op_code = msg_local.operation_code;

  switch (op_code)
  {

  case 0: // Init

    /* Deletion of previous elements */

    result = removelist();

    break;

  case 1: // Set value

    if (exist(msg_local.key) != NULL)
    {
      perror("This key is already inserted!");
      result = -1;
      // Return this value to client
    }

    else
    {
      result = setNode(msg_local.key, msg_local.value1, msg_local.value2);
    }

    break;

  case 2: // Get value

    if (exist(msg_local.key) == NULL)
    {
      perror("This key does not exist!");
      result = -1;
      // Return this value to client
    }

    else
    {
      Node * node = getNode(msg_local.key);

      strcpy(msg_local.value1, node->value1); // Value 1 of the node to be returned in message
      msg_local.value2 = msg_local.value2; // Value 2 of the node to be returned in message

      result = 0; // Value properly obtained
    }

    break;

  case 3: // Modify value

    if (exist(msg_local.key) == NULL)
    {
      perror("This key does not exist!");
      result = -1;
      // Return this value to client
    }

    else
    {
      result = modifyNode(msg_local.key, msg_local.value1, msg_local.value2);
    }

    break;

  case 4: // Delete value by key

    if (exist(msg_local.key) == NULL)
    {
      perror("This key does not exist!");
      result = -1;
      // Return this value to client
    }

    result = deleteByKey(msg_local.key);    

    break;

  case 5: // Value exists

    if(exist(msg_local.key) != NULL){
      result = 1;
    }
    else {
      result = 0;
    }
    break;

  case 6: // Number of items

    result = getCardinality();

    break;

  default:
    break;
  }

  /* Return result to client by sending it to queue */

  /* Wake up server */
  message_not_copied = FALSE; /* FALSE = 0 */

  pthread_cond_signal(&cond_msg);

  pthread_mutex_unlock(&mutex_msg);

  q_client = mq_open(msg_local.q_name, O_WRONLY);

  msg_local.operation_code = result; // Introducing the result of processing in operation code of the response

  printf("Code from server: %d\n", result);

  printf("MESSAGE TO BE SENT\n");
  printf("Operation code: %d\n", msg_local.operation_code);
  printf("Key: %s\n", msg_local.key);
  printf("Value1: %s\n", msg_local.value1);
  printf("Value2: %f\n", msg_local.value2);

  if (q_client == -1)
  {
    perror("Can’t open client queue");
  }

  else
  {
    mq_send(q_client, (char *)&msg_local, sizeof(struct message), 0);
    printf("MESSAGE SENT\n");
    mq_close(q_client);
    printf("QUEUE CLOSED\n");
    printf("\n********************\n");
  }

  pthread_exit(0);
}