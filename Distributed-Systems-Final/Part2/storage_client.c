/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#include "storage.h"


void
storage_1(char *host)
{
	CLIENT *clnt;
	enum clnt_stat retval_1;
	void *result_1;
	enum clnt_stat retval_2;
	char *result_2;
	char *recovertuple_1_topic;
	enum clnt_stat retval_3;
	int result_3;
	char *addtuple_1_topic;
	char *addtuple_1_text;

#ifndef	DEBUG
	clnt = clnt_create (host, STORAGE, STORAGEVER, "tcp"); // Use tcp instead of udp
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}
#endif	/* DEBUG */

	retval_1 = initializestorage_1(&result_1, clnt);
	if (retval_1 != RPC_SUCCESS) {
		clnt_perror (clnt, "call failed");
	}
	retval_2 = recovertuple_1(recovertuple_1_topic, &result_2, clnt);
	if (retval_2 != RPC_SUCCESS) {
		clnt_perror (clnt, "call failed");
	}
	retval_3 = addtuple_1(addtuple_1_topic, addtuple_1_text, &result_3, clnt);
	if (retval_3 != RPC_SUCCESS) {
		clnt_perror (clnt, "call failed");
	}
#ifndef	DEBUG
	clnt_destroy (clnt);
#endif	 /* DEBUG */
}


int
main (int argc, char *argv[])
{
	char *host;

	if (argc < 2) {
		printf ("usage: %s server_host\n", argv[0]);
		exit (1);
	}
	host = argv[1];
	storage_1 (host);
exit (0);
}
