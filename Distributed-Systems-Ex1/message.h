#define MAXSIZE 256

struct message {
  int operation_code; // Code of the operation to be performed in the server
  char key [MAXSIZE];
  char value1 [MAXSIZE]; // +1 due to 0 code --> end of string
  float value2;
  char q_name[MAXSIZE];  
};
