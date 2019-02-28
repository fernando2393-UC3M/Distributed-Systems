#include <stdio.h>
#include <mqueue.h>
#include <string.h>
#include "request.h"

mqd_t q_server; /* Server message queue */
mqd_t q_client; /* Client message queue */

struct request req;
struct request res;
struct mq_attr q_attr;

struct node_t *head;

int init()
{

  /* Deletion of previous elements */

  while (head != null)
  {

    struct node_t *temp = head;
    head = head->next;

    free(temp);
  }
}

int set_value(char *key, char *value1, float value2)
{
}

int get_value(char *key, char *value, float value2)
{
}

int modify_value(char *key, char *value1, float *value2)
{
}

int delete_key(char *key)
{

  if (exist(key) == 0)
  {
    return -1;
  }

  else if (strcmp(head->data->key, key) == 0)
  { // Check first element

    struct node_t *temp = head;
    head = head->next;

    free(temp);

    return 0;
  }

  else
  {

    struct node_t *current = head;
    struct node_t *previous = NULL;

    while (current != null)
    {

      if (strcmp(head->data->key, key) == 0)
      { // Key found

        previous->next = current->next;

        free(current);

        return 0;
      }

      previous = current;
      current = current->next;
    }
  }
}

int exist(char *key)
{

  struct node_t *dummy = head;

  while (dummy != null)
  {

    if (strcmp(dummy->data->key, key) == 0)
    {
      return 1; // Key exists
    }
    else
    {
      dummy = dummy->next;
    }
  }

  return 0; // Key does not exist
}

int num_items()
{

  struct node_t *dummy = head;
  int counter = 0;

  while(dummy != null) {

    counter++;

  }

  return counter;

}
