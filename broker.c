#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include "sll.c"
#include "constants.h"

/* Thread variables */

pthread_mutex_t mutex;
pthread_cond_t condition;
int busy;

void print_usage()
{
	printf("Usage: broker -p port \n");
}

void *manage_request (int* s) {

	int s_local;
	int position = 0;
	int aux = 0;
	int client_socket = 0;
	char buffer[sizeof(OPERATION_PUBLISH)+TOPIC_SIZE+TEXT_SIZE];
	char operation[sizeof(OPERATION_PUBLISH)];
	char topic[TOPIC_SIZE];
	char text[TEXT_SIZE];
	memset(topic, '\0', sizeof(topic));
	memset(text, '\0', sizeof(text));

	pthread_mutex_lock(&mutex);
	s_local = *s;
	busy = FALSE;
	pthread_cond_signal(&condition);
	pthread_mutex_unlock(&mutex);

	/* Request management */

	if(read(*s, buffer, sizeof(buffer)) < 0){
		perror("Broker error: error reading message");
	}

	for(int i = 0; buffer[i]!='\0'; i++){ // Advance until reach topic
		operation[i] = buffer[i];
		position++;
	}

	/* Case PUBLISH */

	if(!strcmp(operation, OPERATION_PUBLISH)){

		position++;

		for (int i = position; buffer[i] != '\0'; i++) // Extract topic
		{ // Advance until end topic
			topic[aux] = buffer[i];
			aux++;
			position++;
		}

		aux = 0;
		position++;

		for (int i = position; buffer[i] != '\0'; i++) // Extract text
		{ // Advance until end of buffer content
			text[aux] = buffer[i];
			aux++;
			position++;
		}

		/* Check if topic is in the topic list, if not, add */

		int check = FALSE;
		int counter = 0;

		for (int i = 0; topiclist[i] != NULL; i++)
		{ // If it is in list, confirm
			if (!strcmp(topic, topiclist[i]))
			{
				check = TRUE; // Set to true the checker
			}
			counter++; // Count the number of elements in list
		}

		if (check == FALSE)
		{ // If it is not in the list, include
			topiclist[counter] = malloc(sizeof(topic));
			strcpy(topiclist[counter], topic);
		}

		/* End of topic check */

		/* Now send the message to all who are subscribed to the topic */

		for(Node * dummy = head; dummy != NULL; dummy = dummy->next) {
			if(isSubscribed(dummy, topic) == 0){
				send(dummy->key, text, sizeof(text), 0); // Send topic text to all subscribed
			}
		}
	}

	/* Case SUBSCRIBE */

	else if(!strcmp(operation, OPERATION_SUBSCRIBE)){

		position++;

		char * socket_buffer = NULL;

		for (int i = position; buffer[i] != '\0'; i++) // Extract topic
		{ // Advance until end topic
			socket_buffer[i] = (char) malloc(sizeof(buffer[i]));
			socket_buffer[i] = buffer[i];
			position++;
		}

		char * ptr;
		client_socket = strtol(socket_buffer, &ptr, 10); // Socket of the client

		position++;

		for (int i = position; buffer[i] != '\0'; i++) // Extract topic
		{ // Advance until end topic
			topic[aux] = buffer[i];
			aux++;
			position++;
		}

		int check = FALSE;

		for (int i = 0; topiclist[i] != NULL; i++) // Check if the topic exists
		{ 
			if (!strcmp(topic, topiclist[i]))
			{
				check = TRUE; // Set to true the checker
				break;
			}
		}

		if(check==FALSE) // If topic to subscribe does not exist
		{
			perror("Broker error: topic does not exist");
		}

		else if(getNode(client_socket)==NULL && check==TRUE) // Node does not exist but topic does --> Include
		{ 
			Node * node = createNewNode(client_socket, topic); // Create new node
			setNode(node); // Add node to the list

		}
		else if(getNode(client_socket)!=NULL && check==TRUE){

			Node * node = getNode(client_socket);
			if(isSubscribed(node, topic)<0)
			{				
				addTopic(node, topic);
			}
			else {
				perror("Broker error: subscriber already subscribed to topic");
			}
		}
	}

	/* Case UNSUBSCRIBE */

	else if(!strcmp(operation, OPERATION_UNSUBSCRIBE)){

		position++;

		char * socket_buffer = NULL;

		for (int i = position; buffer[i] != '\0'; i++) // Extract topic
		{ // Advance until end topic
			socket_buffer[i] = (char) malloc(sizeof(buffer[i]));
			socket_buffer[i] = buffer[i];
			position++;
		}

		char * ptr;
		client_socket = strtol(socket_buffer, &ptr, 10); // Socket of the client

		Node * node = getNode(client_socket);

		position++;

		for (int i = position; buffer[i] != '\0'; i++) // Extract topic
		{ // Advance until end topic
			topic[aux] = buffer[i];
			aux++;
			position++;
		}

		if(isSubscribed(node, topic)==0)
		{
			removeTopic(node, topic);
		}
	}

	/* End of request management */

	close(s_local);
	pthread_exit(NULL);

}

int main(int argc, char *argv[])
{
	int option = 0;
	char port[256] = "";

	while ((option = getopt(argc, argv, "p:")) != -1)
	{
		switch (option)
		{
		case 'p':
			strcpy(port, optarg);
			break;
		default:
			print_usage();
			exit(-1);
		}
	}
	if (strcmp(port, "") == 0)
	{
		print_usage();
		exit(-1);
	}

	printf("Port: %s\n", port);
	

	/* Server socket initialization */

	struct sockaddr_in server_addr, client_addr;
	int sd, sc;
	// int size;
	int val;
	// int num[2], res;
	int client_len = sizeof(client_addr);

	sd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	val = 1;
	setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, (char *) &val, sizeof(int));

	bzero((char*) &server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(atoi(port));

	if(bind(sd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0){
		perror("Broker error: Bind failed");
		return -1;
	}

	if(listen(sd, MAX_CONNECTIONS) < 0){ // Listen up to MAX_CONNECTIONS simultaneous connections
		perror("Broker error: Server cannot listen");
		return -1;
	}

	/* Server is created and listening --> Assign it to a thread */

	pthread_t thid; // Thread identifier
	pthread_attr_t attr; // Thread attributes
	pthread_attr_init(&attr);

	/* Initialize mutex */

	if (pthread_mutex_init(&mutex, NULL) != 0)
    {
        perror("Failure in mutex initialization");
        return 1;
    }

	if (pthread_cond_init(&condition, NULL) != 0)
	{
		perror("Failure in condition initialization");
        return 1;
	}

	for(;;) {

		if ((sc = accept(sd, (struct sockaddr *) &client_addr, (socklen_t*)&client_len)) < 0) {
			perror("Broker error: Request could not be accepted");
			return -1;
		}
		pthread_create(&thid, &attr, (void*) &manage_request, &sc);

		/* Waiting for thread to manage the request */

		pthread_mutex_lock(&mutex);
		while(busy == TRUE) {
			pthread_cond_wait(&condition, &mutex);
		}
		pthread_mutex_unlock(&mutex);
		busy = TRUE;
	}

	return 0;
}