#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

void print_usage() {
	    printf("Usage: editor -h host -p puerto -t \"tema\" -m \"texto\"\n");
}

int main(int argc, char *argv[]) {
	int  option = 0;
	char host[256]= "";
	char puerto[256]= "";
	char tema[256]= "";
	char texto[1024]= "";

	if(argc != 4 ){ /* IP address & port number*/
	   puts("Arguments to the editor must be: ./editor -h host-broker -p port -t topic -m text");
		 return -1;
	}

	/* Variables declaration */
  struct hostent *host;
	struct sockaddr_in server_address;  /* broker address */

	/*Socket creation*/
		int sd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (sd < 0)
		 {
		   perror("Error declaring socket");
		   return -1;
		 }

		host = gethostbyname(argv[2]);
		memcpy(&(server_address.sin_addr), host->h_addr, host->h_length);

	  server_address.sin_family = AF_INET;
	  server_address.sin_port = htons(atoi(argv[2]));
	  server_address.sin_addr.s_addr = inet_aton(argv[1]);
	  bzero(&(server_address.sin_zero), 8);

	/*Connection*/
	if (connect(sd, (struct sockaddr *) &server_address, sizeof(server_address)) < 0)
	{
	perror("Error connecting");
	return -1;
	}
	else
	{
	printf("Successfully connected.\n");
	}




	while ((option = getopt(argc, argv,"h:p:t:m:")) != -1) {
		switch (option) {
		        case 'h' :
				strcpy(host, optarg);
		    		break;
		    	case 'p' :
				strcpy(puerto, optarg);
		    		break;
		    	case 't' :
				strcpy(tema, optarg);
		    		break;
		    	case 'm' :
				strcpy(texto, optarg);
		    		break;
		    	default:
				print_usage();
		    		exit(-1);
		    }
	}
	if (strcmp(host,"")==0 || strcmp(puerto,"")==0 ||
		strcmp(tema,"")==0 || strcmp(texto,"")==0){
		print_usage();
		exit(-1);
	}


	printf("Host: %s\n", host);
	printf("Puerto: %s\n", puerto);
	printf("Tema: %s\n", tema);
	printf("texto: %s\n", texto);




	return 0;
}
