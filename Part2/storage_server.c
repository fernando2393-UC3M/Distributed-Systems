/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#include "storage.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TOPIC_SIZE 128
#define TEXT_SIZE 1024

bool_t
initializestorage_1_svc(void *result, struct svc_req *rqstp)
{
	bool_t retval;

	/*
	 * insert server code here
	 */

	return retval = RPC_SUCCESS;
}

bool_t
recovertuple_1_svc(char *topic, char **result,  struct svc_req *rqstp)
{
	bool_t retval;
	/*
	 * insert server code here
	 */

	FILE *fp;

	char *filepath;
	filepath = malloc(10 + strlen(topic)); // 10 is the number of characters of "data/.txt\n"

	snprintf(filepath, 10 + strlen(topic), "data/%s.txt\n", topic);

	fp = fopen(filepath, "r"); // Open file for reading

	char text [TEXT_SIZE];

	while(fgets(text, sizeof(text), fp) != NULL); // This will read and include in the buffer the content of the last line

	fclose(fp);

	*result = malloc(TEXT_SIZE);

	strcpy(*result, text); // Copy the result text to result

	return retval = RPC_SUCCESS; // Return last message of topic
}

bool_t
addtuple_1_svc(char *topic, char *text, int *result,  struct svc_req *rqstp)
{
	bool_t retval;

	/*
	 * insert server code here
	 */

	/* Check if file exists */

	FILE *fp;

	char *filepath;
	filepath = malloc(10 + strlen(topic)); // 10 is the number of characters of "data/.txt\n"

	snprintf(filepath, 10 + strlen(topic), "data/%s.txt\n", topic);

	fp = fopen(filepath, "w"); // Open file for writing

	if (fp == NULL) { // Error opening/creating file
		perror("Error opening/creating file");
		*result = -1;
	}

	fprintf(fp, "%s", text); // Write text into file
	fprintf(fp, "\n"); // New line

	fclose(fp);

	*result = 0;

	return retval = RPC_SUCCESS;
}

int
storage_1_freeresult (SVCXPRT *transp, xdrproc_t xdr_result, caddr_t result)
{
	xdr_free (xdr_result, result);

	/*
	 * Insert additional freeing code here, if needed
	 */

	return 1;
}
