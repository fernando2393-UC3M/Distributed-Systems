#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "constants.h"

/* List of topics */

char * topiclist [TOPIC_SIZE];

/* SSL Definition */

typedef struct Node
{
    int key;
    char * topics [TOPIC_SIZE];
    struct Node *next; // Next node
} Node;

Node *head; // Head node of list

int removeList()
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

Node * createNewNode(int key, char *topic){

    Node* newNode = (Node*)malloc(sizeof(Node));

    newNode->key = key;

    newNode->topics[0] = malloc(TOPIC_SIZE);
    strcpy(newNode->topics[0], topic);

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

Node *getNode(int key)
{

    Node *dummy = head;

    if(head == NULL) {
        return NULL;
    }

    while (dummy != NULL)
    {
        if(dummy->key == key){
            return dummy;
        }
        dummy = dummy->next;
    }

    return NULL;
}

int modifyNode(Node * node){

    Node * dummy = head;

    while(dummy != NULL){
        if(dummy->key == node->key){

            int counter = 0;

            for(int i = counter; node->topics[i]!=NULL; i++){
                if(dummy->topics[i]!=NULL){

                    if (strlen(dummy->topics[i]) != strlen(node->topics[i])){ // If sizes are different, free and allocate with new size
                        free(dummy->topics[i]);
                        dummy->topics[i] = malloc(sizeof(node->topics[i]));
                    }
                    strcpy(dummy->topics[i], node->topics[i]); // Copy new version of the node
                    counter++;
                }
                else{
                    dummy->topics[i] = malloc(sizeof(node->topics[i])); // Allocate space for new topics
                    strcpy(dummy->topics[i], node->topics[i]);
                    counter++;
                }
            }

            for(int i = counter; dummy->topics[i]!=NULL; i++){ // Free topics which do not appear in updated node
                free(dummy->topics[i]);
            }
            return 0;
        }

        dummy = dummy->next;
    }

    return -1;
}

int deleteByKey(int key) {

    Node* dummy = head;

    /* First check of head */

    if(dummy->key == key) {
                head = dummy->next;
                free(dummy);
                return 0;
            }

    /* If not in head position */

    Node * prev_dummy = dummy;

    dummy = dummy->next;

    while(dummy != NULL) {
        if(dummy->key == key) {

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

int isSubscribed(Node * node, char * topic) {

    for(int i = 0; node->topics[i] != NULL; i++){
        if(!strcmp(node->topics[i], topic)){
            return 0;
        }
    }
    return -1;
}