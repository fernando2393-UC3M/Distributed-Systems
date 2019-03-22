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
	
