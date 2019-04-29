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
#include "lines.h"
#include <netdb.h>
#include <arpa/inet.h>

/* List of topics */

char * topiclist [TOPIC_SIZE];

/* Thread variables */

pthread_mutex_t mutex;
pthread_cond_t condition;
int busy;

void print_usage()
{
	printf("Usage: broker -p port \n");
}

void *manage_request (int* s) {

	char buffer[TEXT_SIZE];
	char * operation = NULL;
	char * topic = NULL;
	char * text = NULL;

	pthread_mutex_lock(&mutex);
	busy = FALSE;
	pthread_cond_signal(&condition);
	pthread_mutex_unlock(&mutex);

	/* Request management */

	if(readLine(*s, buffer, sizeof(buffer)) < 0){
		perror("Broker error: error reading message");
	}

	/* Read operation */

	operation = malloc(strlen(buffer));
	strcpy(operation, buffer);

	/* Case PUBLISH */

	if(!strcmp(operation, OPERATION_PUBLISH)){

		/* Read topic */

		if(readLine(*s, buffer, sizeof(buffer)) < 0){
			perror("Broker error: error reading message");
		}

		topic = malloc(strlen(buffer));
		strcpy(topic, buffer);

		/* Read text */

		if(readLine(*s, buffer, sizeof(buffer)) < 0){
			perror("Broker error: error reading message");
		}

		text = malloc(strlen(buffer));
		strcpy(text, buffer);

		/* Check if topic is in the topic list, if not, add */

		int check = FALSE;
		int counter = 0;

		while(topiclist[counter]!=NULL){
			if (!strcmp(topic, topiclist[counter]))
			{	
				check = TRUE; // Set to true the checker
				break;
			}
			counter++;
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
				
				int sd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

				struct sockaddr_in server_addr;
				struct hostent * hp;

				bzero((char *)&server_addr, sizeof(server_addr));
				hp = gethostbyname("127.0.0.1"); // It is necessary to parse the IP --> now it is 0.0.0.0/0.0.0.0

				memcpy(&(server_addr.sin_addr), hp->h_addr, hp->h_length);

				server_addr.sin_family = AF_INET;
				server_addr.sin_port = htons(dummy->port);

				connect(sd, (struct sockaddr *) &server_addr, sizeof(server_addr));
				
				if (send(sd, topic, sizeof(topic), 0) == -1) {
					perror("Error sending topic");
				}

				if (send(sd, text, sizeof(text), 0) == -1) {
					perror("Error sending text");
				}
			}
		}
	}

	/* Case SUBSCRIBE */

	else if(!strcmp(operation, OPERATION_SUBSCRIBE)){

		/* Read topic */

		if(readLine(*s, buffer, sizeof(buffer)) < 0){
			perror("Broker error: error reading message");
		}

		topic = malloc(strlen(buffer));
		strcpy(topic, buffer);

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

			send(*s, "1", sizeof("1"), 0);
			
		}

		else
		{

			/* Read address */

			if (readLine(*s, buffer, sizeof(buffer)) < 0)
			{
				perror("Broker error: error reading message");
			}

			char *string_address = malloc(strlen(buffer));
			strcpy(string_address, buffer);

			/* Read port */

			if (readLine(*s, buffer, sizeof(buffer)) < 0)
			{
				perror("Broker error: error reading message");
			}

			char *string_port = malloc(strlen(buffer));
			strcpy(string_port, buffer);

			Node * node = getNode(atoi(string_port), string_address);

			if(node == NULL) { // Node does not exist --> Create

				node = createNewNode(atoi(string_port), string_address, topic);
				setNode(node);

			}

			else {
				addTopic(node, topic); // If node does exist --> Add topic
			}

			send(*s, "0", sizeof("0"), 0);

		}
	}

	/* Case UNSUBSCRIBE */

	else if(!strcmp(operation, OPERATION_UNSUBSCRIBE)){

		/* Read topic */

		if(readLine(*s, buffer, sizeof(buffer)) < 0){
			perror("Broker error: error reading message");
		}

		topic = malloc(strlen(buffer));
		strcpy(topic, buffer);

		int check = FALSE;

		for (int i = 0; topiclist[i] != NULL; i++) // Check if the topic exists
		{ 
			if (!strcmp(topic, topiclist[i]))
			{	
				check = TRUE; // Set to true the checker
				break;
			}
		}

		if(check==FALSE) // If topic to unsubscribe does not exist
		{

			send(*s, "1", sizeof("1"), 0);
			
		}

		else {

			/* Read address */

			if (readLine(*s, buffer, sizeof(buffer)) < 0)
			{
				perror("Broker error: error reading message");
			}

			char *string_address = malloc(strlen(buffer));
			strcpy(string_address, buffer);

			/* Read port */

			if (readLine(*s, buffer, sizeof(buffer)) < 0)
			{
				perror("Broker error: error reading message");
			}

			char *string_port = malloc(strlen(buffer));
			strcpy(string_port, buffer);

			Node * node = getNode(atoi(string_port), string_address);

			if(node == NULL) { // Node does not exist --> Cannot unsubscribe

				send(*s, "1", sizeof("1"), 0);
			}

			else
			{
				removeTopic(node, topic); // If node does exist --> Add topic
				send(*s, "0", sizeof("0"), 0);
			}
		}
	}

	/* End of request management */

	close(*s);
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