#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

#define TOPIC_SIZE 128
#define TEXT_SIZE 1024
#define OPERATION "PUBLISH"


void print_usage() {
	printf("Usage: editor -h host -p port -t \"topic\" -m \"text\"\n");
}

int main(int argc, char *argv[]) {
	int  option = 0;
	char host[256]= "";
	char port[256]= "";
	char topic[TOPIC_SIZE]= "";
	char text[TEXT_SIZE]= "";

	memset(topic, '\0', sizeof(topic)); // Add \0 in the end of string
	memset(text, '\0', sizeof(text)); // Add \0 in the end of string

	if (argc != 9)
	{ /* IP address & port number*/
		printf("There are %d arguments!\n", argc);
		perror("Arguments to the editor must be: ./editor -h host-broker -p port -t topic -m text");
		return -1;
	}

	/* Variables declaration */
  struct hostent *hp;
	struct sockaddr_in server_address;  /* broker address */

	while ((option = getopt(argc, argv, "h:p:t:m:")) != -1)
	{
		switch (option)
		{
		case 'h':
			strcpy(host, optarg);
			break;
		case 'p':
			strcpy(port, optarg);
			break;
		case 't':
			strcpy(topic, optarg);
			break;
		case 'm':
			strcpy(text, optarg);
			break;
		default:
			print_usage();
			exit(-1);
		}
	}

	if (strcmp(host,"")==0 || strcmp(port,"")==0 ||
		strcmp(topic,"")==0 || strcmp(text,"")==0){
		print_usage();
		exit(-1);
	}

	printf("Host: %s\n", host);
	printf("Port: %s\n", port);
	printf("Topic: %s\n", topic);
	printf("Text: %s\n", text);

	/*Socket creation*/
	int sd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	
	if (sd < 0)
	{
		perror("Error declaring socket");
		return -1;
	}

	bzero((char *)&server_address, sizeof(server_address));
	
	hp = gethostbyname(host);
	memcpy(&(server_address.sin_addr), hp->h_addr, hp->h_length);

	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(atoi(port)); 

	/*Connection*/

	if (connect(sd, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
	{
		fprintf(stderr, "Error in the connection to the server %s:%s\n", host, port);
		return -1;
	}
	else
	{
		printf("Successfully connected to the server\n");
	}

	/* Editor communication */

	/* Send packet to broker */

	/* Get topic length */

	int counter = 1; // Due to '\0'

	for(int i = 0; i<TOPIC_SIZE; i++) {
		if(topic[i] != '\0'){
			counter++;
		}
	}

	char final_topic[counter];
	memset(final_topic, '\0', sizeof(final_topic));

	for(int i = 0; i<sizeof(final_topic); i++){
		final_topic[i]=topic[i];
	}

	/* Get text length */

	counter = 1; // Due to '\0'

	for(int i = 0; i<TEXT_SIZE; i++) {
		if(text[i] != '\0'){
			counter++;
		}
	}

	char final_text[counter];
	memset(final_text, '\0', sizeof(final_text));

	for(int i = 0; i<sizeof(final_text); i++){
		final_text[i]=text[i];
	}

	if (send(sd, OPERATION, sizeof(OPERATION), 0) == -1) {
		perror("Error sending operation code");
		return -1;
	}

	if (send(sd, final_topic, sizeof(final_topic), 0) == -1) {
		perror("Error sending topic");
		return -1;
	}

	if (send(sd, final_text, sizeof(final_text), 0) == -1) {
		perror("Error sending text");
		return -1;
	}

	printf("All messages have been sent\n");

	return 0;
}
