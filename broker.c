#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

/* Constants */

#define FALSE 0
#define TRUE 1
#define MAX_CONNECTIONS 3

/* Editor packet components size */

#define TOPIC_SIZE 128
#define TEXT_SIZE 1024
#define OPERATION "PUBLISH"

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
	char buffer[sizeof(OPERATION)+TOPIC_SIZE+TEXT_SIZE];
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
		position++;
	}

	position++;

	for(int i = position; buffer[i]!='\0'; i++){ // Advance until end topic
		topic[aux] = buffer[i];
		aux++;
		position++;
	}

	aux = 0;
	position++;

	for(int i = position; buffer[i]!='\0'; i++){ // Advance until end of buffer content
		text[aux] = buffer[i];
		aux++;
		position++;
	}

	printf("Topic: %s\n", topic);
	printf("Text: %s\n", text);

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