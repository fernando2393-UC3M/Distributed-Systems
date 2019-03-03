#include <stdio.h>
#include <mqueue.h>
#include <string.h>
#include "message.h"

#define SERVER_QUEUE "/SERVER_QUEUE"
#define CLIENT_QUEUE "/CLIENT_QUEUE"

struct message req;
struct message res;
struct mq_attr q_attr;

/* Definition of messagePassing function */

int messagePassing(int operation_code, char * key, char * value1, float value2, struct message * destination);

int init()
{
  // Send message with op.code 0 to the server

  struct message msg_local;

  return messagePassing(0, "", "", 0, &msg_local); 
}

int set_value(char *key, char *value1, float value2)
{
  // Send message with op.code 1 to the server

  struct message msg_local;

  return messagePassing(1, key, value1, value2, &msg_local);
}

int get_value(char *key, char *value1, float *value2)
{
  // Send message with op.code 2 to the server

  struct message msg_local;

  if(messagePassing(2, key, "", 0, &msg_local) == 0) { // Message processing properly done
    strcpy(value1, msg_local.value1);
    *value2 = msg_local.value2;
    return 0;
  }

  return -1;  
}

int modify_value(char *key, char *value1, float *value2)
{
  // Send message with op.code 3 to the server

  struct message msg_local;

  return messagePassing(3, key, value1, *value2, &msg_local);

}

int delete_key(char *key)
{
  // Send message with op.code 4 to the server

  struct message msg_local;

  return messagePassing(4, key, "", 0, &msg_local);

}

int exist(char *key)
{
  // Send message with op.code 5 to the server

  struct message msg_local;

  return messagePassing(5, key, "", 0, &msg_local);

}

int num_items()
{
  // Send message with op.code 6 to the server

  struct message msg_local;

  return messagePassing(6, "", "", 0, &msg_local);
}

int messagePassing(int operation_code, char * key, char * value1, float value2, struct message * destination){

  mqd_t q_server; /* Server message queue */
  mqd_t q_client; /* Client message queue */

  struct message request; /* Request declaration */

  /* Attributes for client queue */
  struct mq_attr attr;

  /* Maximum number of messages --> 1 */
    attr.mq_maxmsg = 1;
    attr.mq_msgsize = sizeof(struct message);

    /* Open client and server queues */
    q_client = mq_open(CLIENT_QUEUE, O_CREAT|O_RDWR, 0660, &attr);
    q_server = mq_open(SERVER_QUEUE, O_WRONLY);

    /* Fill the request */

    strcpy(request.q_name, CLIENT_QUEUE); //
    request.operation_code = operation_code;
    strcpy(request.key, key);
    memset(request.value1, '\0', sizeof(request.value1));
    strcpy(request.value1, value1);
    request.value2 = value2;

    printf("Request sent:\n");
    printf("Operation Code: %d\n", request.operation_code);
    printf("Key: %s\n", request.key);
    printf("Value1: %s\n", request.value1);
    printf("Value2: %f\n\n", request.value2);

    /* Send request to server */
    printf("Sending request... ");
    mq_send(q_server, (char*) &request, sizeof(struct message), 0);
    printf("Request sent.\n");

    /* Receiving response */
    printf("Waiting for response... ");
    mq_receive(q_client, (char*) destination, sizeof(struct message), 0);
    printf("Response received.\n");

    /* Close the queues */
    mq_close(q_server);
    mq_close(q_client);

    /* Unlink client queue */
    mq_unlink(CLIENT_QUEUE);

    return destination->operation_code;

}
