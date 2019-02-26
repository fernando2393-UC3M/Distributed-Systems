#define MAXSIZE	256
#define TRUE	1
#define FALSE   0
#define serverq "/SERVER"

struct request {
  char k[MAXSIZE];
  char v[MAXSIZE];
  char q_name[MAXSIZE];
  int f;
  float v2;
  int status;
  char resp[MAXSIZE];
};
