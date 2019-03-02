#define MAXSIZE	255
#define TRUE 1
#define FALSE 0
#define serverq "/SERVER"

struct request {
  int operation_code; // Code of the operation to be performed in the server
  char key [MAXSIZE];
  char value1 [MAXSIZE + 1]; // +1 due to 0 code --> end of string
  float value2;
  char q_name[MAXSIZE];  
};

/* SSL Definition */

typedef struct node {
        struct request data; // Value of type request
        struct node * next; // Next node
    } * node_t;
