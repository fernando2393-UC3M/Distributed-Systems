#include <mqueue.h>
#include <string.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "request.h"

/* mutex and condition variables for the message copy */

pthread_mutex_t mutex_msg;
int message_not_copied = TRUE;
pthread_cond_t cond_msg;

int main(void) {

  /* Server data */

  mqd_t q_server; /* server queue */
  struct request msg; /* message to receive */
  struct mq_attr q_attr; /* queue atributes */
  pthread_attr_t t_attr; /* thread atributes */

  /* Queue size */

  q_attr.mq_maxmsg = 20;
  q_attr.mq_msgsize = sizeof(struct request));

  /* List initialization */

  node_t * head = NULL;
  head = malloc(sizeof(node_t));

  request triplet = NULL;

  head->value = triplet;
  head->next = NULL;

  /* Initialization */

  q_server = mq_open(“SERVER”, O_CREAT|O_RDONLY, 0700, &attr);
  if (q_server == -1) {
    perror(”Can’t create server queue”);
    return 1; 
  }

  pthread_mutex_init(&mutex_msg, NULL);
  pthread_cond_init(&cond_msg, NULL);
  pthread_attr_init(&attr);

  /* Thread atributes */
  pthread_attr_setdetachstate(&t_attr, PTHREAD_CREATE_DETACHED);

  while (TRUE) {
    mq_receive(q_server, &msg, sizeof(struct request), 0);
    pthread_create(&thid, &attr, process_message, &msg);

    /* Critical Section --> message copying */
    
    pthread_mutex_lock(&mutex_msg);

    while (message_not_copied) {
      pthread_cond_wait(&cond_msg, &mutex_msg);    
    }

    message_not_copied = TRUE;
    pthread_mutex_unlock(&mutex_msg);

  }
}

void process_message(struct mensaje *msg) {

  struct request msg_local;
  struct mqd_t q_client;
  int result;

  /* Thread copies message to local message */
  pthread_mutex_lock(&mutex_msg);

  memcpy((char *) &msg_local, (char *)&msg, sizeof(struct request));

  /* wake up server */
  message_not_copied = FALSE; /* FALSE = 0 */

  pthread_cond_signal(&cond_msg);

  pthread_mutex_unlock(&mutex_msg);

  /* Execute client request and prepare reply */ 
  
  result = msg_local.a + msg_local.b;

  /* Return result to client by sending it to queue */ 
  
  q_client = mq_open(msg_local.name, O_WRONLY);

  if (q_client == -1) {
    perror("Can’t open client queue");
  }

  else {
    mq_send(q_client, (char *) &result, sizeof(int), 0); mq_close(q_client);
  }

  pthread_exit(0);
}
