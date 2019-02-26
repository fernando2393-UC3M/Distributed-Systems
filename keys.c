#include <stdio.h>
#include <mqueue.h>
#include <string.h>
#include "request.h"

mqd_t q_server; /* server message queue */
mqd_t q_client; /* client message queue */

struct request req;
struct request res;
struct mq_attr q_attr;

int init() {
  q_attr.mq_curmsgs = 0;
  q_attr.mq_flags = 0;
  q_attr.mq_maxmsg = 4;
  q_attr.mq_msgsize = sizeof(struct request);
  q_client = mq_open("/CLIENT_INIT", O_CREAT|O_RDONLY, 0700, &q_attr);
  q_server = mq_open(serverq, O_WRONLY);

  strcpy(req.q_name, "/CLIENT_INIT");
  req.f = 0;

  mq_send(q_server, (char *) &req, sizeof(struct request), 0);
  mq_receive(q_client, (char *) &res, sizeof(struct request), 0);

  mq_close(q_server);
  mq_close(q_client);
  mq_unlink("/CLIENT_INIT");
  printf("res.status %d\n", res.status);
  return res.status;
}

int set_value(char *key, char *value, float value2) {
  q_attr.mq_curmsgs = 0;
  q_attr.mq_flags = 0;
  q_attr.mq_maxmsg = 4;
  q_attr.mq_msgsize = sizeof(struct request);
  q_client = mq_open("/CLIENT_SET", O_CREAT|O_RDONLY, 0700, &q_attr);
  q_server = mq_open(serverq, O_WRONLY);

  strcpy(req.q_name, "/CLIENT_SET");
  req.k = key;
  strcpy(req.v, value, value2);
  req.f = 1;
  printf("res.status %d\n", res.status);
  mq_send(q_server, (char *) &req, sizeof(struct request), 0);
  mq_receive(q_client, (char *) &res, sizeof(struct request), 0);

  mq_close(q_server);
  mq_close(q_client);
  mq_unlink("/CLIENT_SET");
  return res.status;
}

int get_value(char *key, char *value, float value2) {
  q_attr.mq_curmsgs = 0;
  q_attr.mq_flags = 0;
  q_attr.mq_maxmsg = 4;
  q_attr.mq_msgsize = sizeof(struct request);
  q_client = mq_open("/CLIENT_GET", O_CREAT|O_RDONLY, 0700, &q_attr);
  q_server = mq_open(serverq, O_WRONLY);

  strcpy(req.q_name, "/CLIENT_GET");
  req.k = key;
  req.f = 2;

  mq_send(q_server, (char *) &req, sizeof(struct request), 0);
  mq_receive(q_client, (char *) &res, sizeof(struct request), 0);

  mq_close(q_server);
  mq_close(q_client);
  mq_unlink("/CLIENT_GET");
  printf("res.v %s\n", res.v);
  memcpy(value, res.v, 256);
  return res.status;
}

int modify_value(int key, char *value1, float *value2){

}

int delete_key(int key) {
  q_attr.mq_curmsgs = 0;
  q_attr.mq_flags = 0;
  q_attr.mq_maxmsg = 4;
  q_attr.mq_msgsize = sizeof(struct request);
  q_client = mq_open("/CLIENT_DEL", O_CREAT|O_RDONLY, 0700, &q_attr);
  q_server = mq_open(serverq, O_WRONLY);

  strcpy(req.q_name, "/CLIENT_DEL");
  req.k = key;
  req.f = 3;

  mq_send(q_server, (char *) &req, sizeof(struct request), 0);
  mq_receive(q_client, (char *) &res, sizeof(struct request), 0);

  mq_close(q_server);
  mq_close(q_client);
  mq_unlink("/CLIENT_DEL");

  return res.status;
}

int exist(char *key){

}

int num_items(){

}
