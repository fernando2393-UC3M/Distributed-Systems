#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* SSL Definition */

typedef struct Node
{
    char key[MAXSIZE];
    char value1[MAXSIZE];
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

Node * createNewNode(char * key, char *value1, float value2){

    Node* newNode = (Node*)malloc(sizeof(Node));

    strcpy(newNode->key, key);
    strcpy(newNode->value1, value1);
    newNode->value2 = value2;
    newNode->next = NULL;

    return newNode;

}

int setNode(Node * node)
{

    Node *dummy = head;

    if(head == NULL) {
        head = node;
        return 0;
    }

    while (dummy != NULL)
    {
        if(dummy->next != NULL) {
            dummy = dummy->next;
        }
        else {
            break;
        }
    }
    dummy->next = node;
    return 0;
}

Node *getNode(char *key)
{

    Node *dummy = head;

    if(head == NULL) {
        return NULL;
    }

    while (dummy != NULL)
    {
        if(strcmp(dummy->key, key) == 0){
            return dummy;
        }
        dummy = dummy->next;
    }

    return NULL;
}

int modifyNode(Node * node){

    Node * dummy = head;

    while(dummy != NULL){
        if(strcmp(dummy->key, node->key) == 0){
            strcpy(dummy->value1, node->value1);
            dummy->value2 = node->value2;
            printf("\nValues now are:\n");
            return 0;
        }

        dummy = dummy->next;
    }

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

int getCardinality() {

    Node * dummy = head;
    int counter = 0;

    if(head == NULL) {
        return counter; // Counter is 0
    }

    while(dummy != NULL) {
        counter++;
        dummy = dummy->next;
    }

    return counter;
}