#include "request.h"
#include <mqueue.h>


void main(void) {
   mqd_t q_server;      /* Server message queue */
   mqd_t q_client;      /* Client message queue */

   struct request req;
   int res;
   struct mq_attr attr;

   attr.mq_maxmsg = 1;
   attr.mq_msgsize = sizeof(request);
   q_client = mq_open("CLIENT_ONE", O_CREAT|O_RDONLY, 0700, &attr);

   q_server = mq_open("ADD_SERVER", O_WRONLY);

   /* Fill in request */
   req.key = "Testing";
   req.value1 = "Value1";
   req.value2 = 2.1; 
   strcpy(req.q_name, "CLIENT_ONE");

   mq_send(q_server, &req, sizeof(struct request), 0);
   mq_receive(q_client, &res, sizeof(int), 0);

   mq_close(q_server);
   mq_close(q_client);
   mq_unlink("CLIENT_ONE");

}
