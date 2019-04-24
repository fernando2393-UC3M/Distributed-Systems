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


void print_usage() {
	    printf("Usage: editor -h host -p port -t \"topic\" -m \"text\"\n");
}

int main(int argc, char *argv[]) {
	int  option = 0;
	char host[256]= "";
	char port[256]= "";
	char topic[256]= "";
	char text[1024]= "";

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
	char packet [TOPIC_SIZE + TEXT_SIZE + 1]; // 1 added due to the separation topic:text
	memset(packet, '\0', sizeof(packet));
	sprintf(packet, "%s:%s", topic, text);
	printf("Packet to be sent: %s\n", packet);
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
		fprintf(stderr, "Error in the connection to the server %s:%s", host, port);
		return -1;
	}
	else
	{
		printf("Successfully connected to the server\n");
	}

	/* Send packet to broker */

	if (send(sd, packet, sizeof(packet), 0) == -1) {
		perror("Error sending packet");
		return -1;
	}

	return 0;
}
