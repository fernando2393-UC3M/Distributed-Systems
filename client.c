#include <stdio.h>
#include "keys.h"


int main(void) {

   int outcome;

   /* Triplet values */

   char * key = "Test Set Value";
   char * value1 = "Test Value 1";
   float value2 = 3.14159;

   /* Testing all functions from keys.c API */

   /* Init */

   printf("Init function: \n");
   outcome = init();
   printf("Init outcome: %d\n", outcome);

   /* Set Value */ 

   printf("Set Value function: \n");
   outcome = set_value(key, value1, value2);
   printf("Set Value outcome: %d\n", outcome);

   /* Modify Value */

   value2 = 2.71183;

   printf("Modify Value function: \n");
   outcome = modify_value(key, "Test Modified Value", &value2);
   printf("Modify Value outcome: %d\n", outcome);

   /* Get Value */

   printf("Get Value function: \n");
   outcome = get_value(key, value1, &value2);
   printf("Init outcome: %d\n", outcome);
   printf("Obtained value: %s %s %f", key, value1, value2);

   /* Exists */

   printf("Exists function: \n");
   outcome = exist(key);
   printf("Exists outcome: %d\n", outcome);

   /* Delete Key */

   printf("Delete Key function: \n");
   outcome = delete_key(key);
   printf("Delete Key outcome: %d\n", outcome);

   /* Delete Key */

   printf("Num Items function: \n");
   outcome = num_items();
   printf("Num Items outcome: %d\n", outcome);

   return 0;

}
