#include <stdio.h>
#include <mqueue.h>
#include <string.h>
#include "request.h"

mqd_t q_server; /* Server message queue */
mqd_t q_client; /* Client message queue */

struct request req;
struct request res;
struct mq_attr q_attr;

int init()
{
  // Send message with op.code 0 to the server

  struct request req;
  char *aux;
  int res;

  req.operation_code = 0; // Init code

  mq_send(q_server, &req, sizeof(struct request), 0);
  
  // Wait until response

  mq_receive(q_client, &aux, sizeof(int), 0);
  res = atoi(aux);

  return res;
}

int set_value(char *key, char *value1, float value2)
{
  // Send message with op.code 1 to the server

  struct request req;
  int res;

  req.operation_code = 1; // Init code

  strcpy(key, req.key);
  strcpy(value1, req.value1);
  req.value2 = value2;

  mq_send(q_server, &req, sizeof(struct request), 0);

  // Wait until response

  mq_receive(q_client, &res, sizeof(int), 0);

  return res;
}

int get_value(char *key, char *value, float *value2)
{
  // Send message with op.code 2 to the server

  struct request req;
  int res;

  req.operation_code = 2; // Init code

  // Wait until response
}

int modify_value(char *key, char *value1, float *value2)
{
  // Send message with op.code 3 to the server

  struct request req;
  int res;

  req.operation_code = 3; // Init code

  strcpy(key, req.key);
  strcpy(value1, req.value1);
  req.value2 = value2;

  mq_send(q_server, &req, sizeof(struct request), 0);

  // Wait until response

  mq_receive(q_client, &res, sizeof(int), 0);

  return res;

}

int delete_key(char *key)
{
  // Send message with op.code 4 to the server

  struct request req;
  int res;

  req.operation_code = 4; // Init code

  mq_send(q_server, &req, sizeof(struct request), 0);

  // Wait until response

  mq_receive(q_client, &res, sizeof(int), 0);

  return res;

}

int exist(char *key)
{
  // Send message with op.code 5 to the server

  struct request req;
  int res;

  req.operation_code = 5; // Init code

  mq_send(q_server, &req, sizeof(struct request), 0);

  // Wait until response

  mq_receive(q_client, &res, sizeof(int), 0);

  return res;

}

int num_items()
{
  // Send message with op.code 6 to the server

  struct request req;
  int res;

  req.operation_code = 6; // Init code

  mq_send(q_server, &req, sizeof(struct request), 0);

  // Wait until response

  mq_receive(q_client, &res, sizeof(int), 0);

  return res;
}
