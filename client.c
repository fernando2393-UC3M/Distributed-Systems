#include <stdio.h>
#include <string.h>
#include "keys.h"
#include "message.h"


int main(void) {

   int outcome;

   /* Triplet values */

   char * key = "Test Set Value";
   char value1 [MAXSIZE];
   float value2 = 3.14159;

   strcpy(value1, "Test Value 1");

   /* Testing all functions from keys.c API */

   /* Init (0)*/

   printf("-------------- INIT FUNCTION -------------- \n\n");
   outcome = init();
   printf("Init outcome: %d\n", outcome);

   /* Set Value (1)*/ 

   printf("-------------- SET VALUE FUNCTION -------------- \n\n");
   outcome = set_value(key, value1, value2);
   printf("Set Value outcome: %d\n\n", outcome);


   /* Modify Value (3)*/


   value2 = 2.71183;

   printf("-------------- MODIFY VALUE FUNCTION -------------- \n\n");
   outcome = modify_value(key, "Test Modified Value", &value2);
   printf("Modify Value outcome: %d\n\n", outcome);

   /* Get Value (2)*/

   
   printf("-------------- GET VALUE FUNCTION -------------- \n\n");
   outcome = get_value(key, value1, &value2);
   printf("Get Value outcome: %d\n", outcome);
   printf("Obtained triplet:\nKey:  %s\nValue 1: %s\nValue 2: %f\n\n", key, value1, value2);
   

   /* Exists (5)*/

   printf("-------------- EXISTS FUNCTION -------------- \n\n");
   outcome = exist(key);
   printf("Exists outcome: %d\n\n", outcome);


   /* Delete Key (4)*/

   printf("-------------- DELETE KEY FUNCTION -------------- \n\n");
   outcome = delete_key(key);
   printf("Delete Key outcome: %d\n\n", outcome);


   /* Num Items (6)*/

   printf("-------------- NUM ITEMS FUNCTION -------------- \n\n");
   outcome = num_items();
   printf("Num Items outcome: %d\n\n", outcome);

   return 0;

}
