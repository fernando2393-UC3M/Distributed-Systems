#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "constants.h"
#include <sys/socket.h>

/* SSL Definition */

typedef struct Node
{
    struct sockaddr_in key; // Change to sock addr and store, as in client, which will be unique
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

Node * createNewNode(struct sockaddr_in key, char *topic){

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

Node *getNode(struct sockaddr_in key)
{

    Node *dummy = head;

    if(head == NULL) {
        return NULL;
    }

    while (dummy != NULL)
    {
        if((dummy->key.sin_port == key.sin_port) && (dummy->key.sin_addr.s_addr == key.sin_addr.s_addr)){ // If port is the same and address is also the same
            return dummy;
        }
        dummy = dummy->next;
    }

    return NULL;
}

void addTopic(Node *node, char *topic)
{
    int counter = 0;

    for(int i = 0; node->topics[i]!=NULL; i++){
        counter++;        
    }
    counter++;
    node->topics[counter] = malloc(TOPIC_SIZE);
    strcpy(node->topics[counter], topic);
}

void removeTopic(Node *node, char *topic)
{
	int counter = 0;

	for (int i = 0; node->topics[i] != NULL; i++)
	{
		if (!strcmp(node->topics[i], topic))
		{
			counter = i;
			break;
		}
	}
	counter++;

	if (node->topics[counter] == NULL)
	{
		node->topics[counter - 1] = NULL;
	}

	else
	{

		for (int i = counter; node->topics[i] != NULL; i++)
		{
			strcpy(node->topics[i - 1], node->topics[i]);
			counter = i;
		}

		node->topics[counter] = NULL;
	}
}

int deleteByKey(struct sockaddr_in key) {

    Node* dummy = head;

    /* First check of head */

    if((dummy->key.sin_port == key.sin_port) && (dummy->key.sin_addr.s_addr == key.sin_addr.s_addr)){ // If port is the same and address is also the same
                head = dummy->next;
                free(dummy);
                return 0;
            }

    /* If not in head position */

    Node * prev_dummy = dummy;

    dummy = dummy->next;

    while(dummy != NULL) {
        if((dummy->key.sin_port == key.sin_port) && (dummy->key.sin_addr.s_addr == key.sin_addr.s_addr)){ // If port is the same and address is also the same

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