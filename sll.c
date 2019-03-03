#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXSIZE 255

/* SSL Definition */

typedef struct Node
{
    char key[MAXSIZE];
    char value1[MAXSIZE + 1];
    float value2;
    struct Node *next; // Next node
} Node;

Node *head;

int removelist()
{

    Node *dummy = head;
    Node *temp;

    while (dummy != NULL)
    {
        temp = dummy;
        dummy = dummy->next;
        free(temp);
    }

    head = NULL;

    return 0;
}

int setNode(char *key, char *value1, float value2)
{

    Node *dummy = head;

    while (dummy != NULL)
    {
        dummy = dummy->next;
    }

    dummy = malloc(sizeof(struct Node));
    strcpy(dummy->key, key);
    strcpy(dummy->value1, value1);
    dummy->value2 = value2;
    dummy->next = NULL;

    return 0;
}

Node *getNode(char *key)
{

    Node *dummy = head;

    while (strcmp(dummy->key, key) != 0)
    {
        dummy = dummy->next;
    }

    return dummy;
}

int modifyNode(char *key, char *value1, float value2){
    Node * dummy = getNode(key);
    strcpy(dummy->value1, value1);
    dummy->value2 = value2;

    return 0;
}

int deleteByKey(char * key) {

    Node* dummy = head;

    /* First check of head */

    if(strcmp(dummy->key, key) == 0) {
                head = dummy->next;
                free(dummy);
                return 0;
            }

    /* If not in head position */

    Node * prev_dummy = dummy;

    dummy = dummy->next;

    while(dummy != NULL) {
        if(strcmp(dummy->key, key) == 0) {

            prev_dummy->next = dummy->next;

            free(dummy);

            return 0;
        }
        
        dummy = dummy->next;
        prev_dummy = prev_dummy->next;
    }

    return -1;
}

Node *exist(char *key)
{

    Node *dummy = head;

    if (head == NULL)
    {
        return NULL;
    }

    while (dummy != NULL)
    {
        if (strcmp(dummy->key, key) == 0)
        {
            return dummy;
        }
        dummy = dummy->next;
    }

    return NULL;
}

int getCardinality() {
    int counter;

    Node * dummy = head;

    while(dummy->next != NULL) {
        counter++;
        dummy = dummy->next;
    }

    return counter;
}