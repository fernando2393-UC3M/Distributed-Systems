#define MAXSIZE	255
#define TRUE	1
#define FALSE   0
#define serverq "/SERVER"

struct request {
  char k[MAXSIZE];
  char v1[MAXSIZE];
  char q_name[MAXSIZE];
  int f;
  float v2;
  int status;
  char resp[sizeof(k)+sizeof(v1)+sizeof(v2)]; // This is the response --> triplet of size (s.of(k)+s.of(v1)+s.of(v2))
};
