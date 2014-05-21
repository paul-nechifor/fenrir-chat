#include "client_simplu.h"

void creare_client(char* adresa, int port, int* sd)
{
	struct sockaddr_in server;
	int arg, i, r;
	
	/* cream socket-ul */
	if ((*sd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		die("Eroare la crearea socket-ului in client");

	/* setez socket-ul sa fie non-blocant */
	if ((arg = fcntl(*sd, F_GETFL, NULL)) < 0)
		die("Eroare la citirea flag-urilor.");
	if (fcntl(*sd, F_SETFL, arg | O_NONBLOCK) == -1)
		die("Eroare la setarea flag-urilor.");

	server.sin_family = AF_INET;
	server.sin_addr.s_addr = inet_addr(adresa);
	server.sin_port = htons(port);

	/* ma conectez la server. Timpul dintre incercari creste. */
	for (i = 1<<13, r = -1; i < 1<<23 && r == -1; i<<=1)
	{
		r = connect(*sd, (struct sockaddr*)&server, sizeof(struct sockaddr));
		fprintf(stderr, "Nu am reusit sa ma conectez. Astept %d microsecunde. Mai incerc...\n", i);
		usleep(i);
	}

	if (r == -1) die("Nu am putut sa ma conectez. Renunt.\n");

	printf("M-am conectat.\n");
}
