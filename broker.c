#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

void print_usage()
{
	printf("Usage: broker -p puerto \n");
}

int main(int argc, char *argv[])
{
	int option = 0;
	char puerto[256] = "";

	while ((option = getopt(argc, argv, "p:")) != -1)
	{
		switch (option)
		{
		case 'p':
			strcpy(puerto, optarg);
			break;
		default:
			print_usage();
			exit(-1);
		}
	}
	if (strcmp(puerto, "") == 0)
	{
		print_usage();
		exit(-1);
	}

	printf("Puerto: %s\n", puerto);
	return 0;
}
