#include "server_simplu.h"

#define CLIENTI_SIZE 16
int clienti_nr = 0;
int clienti[CLIENTI_SIZE]; /* Memoreaza sd-ul clientului. Daca e -1, este gol. */
#define BUFFER_SIZE 4096
int buffer_nr = 0;
char buffer[BUFFER_SIZE];

void creare_server(int port)
{
	int sd, arg;
	struct sockaddr_in server, from;

	/* initializez toate sd-urile clientilor cu -1 */
	for (sd = 0; sd < CLIENTI_SIZE; sd++)
		clienti[sd] = -1;

	/* creez socket-ul initial */
	if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		die("Eroare la crearea socket-ului initial.");

	/* setez socket-ul initial sa fie non-blocant */
	if ((arg = fcntl(sd, F_GETFL, NULL)) < 0)
		die("Eroare la citirea flag-urilor.");
	if (fcntl(sd, F_SETFL, arg | O_NONBLOCK) == -1)
		die("Eroare la setarea flag-urilor.");

	/* umplem structura folosita de server */
	memset(&server, 0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	server.sin_port = htons(port); /* portul dat */

	/* atasez socket-ul */
	if (bind(sd, (struct sockaddr*)&server, sizeof(struct sockaddr)) == -1)
		die("Eroare la atasarea socketului. Probabil portul este ocupat de altcineva.");

	/* ascult */
	if (listen(sd, 16) == -1)
		die("Eroare la ascultare.");

	/* Eee, aici e partea grea */
	for (;;)
	{
		int client, i;
		socklen_t lungime = sizeof(from);

		/* incerc sa realizez o conexiune */
		client = accept(sd, (struct sockaddr*)&from, &lungime);
		if (client == -1)
		{
			if (errno != EAGAIN)
				fprintf(stderr, "Eroare la o acceptare. Continui...\n");
		}
		else
		{
			/* o noua conexiune realizata */
			if (clienti_nr == CLIENTI_SIZE)
			{
				fprintf(stderr, "Am numarul maxim de clienti. Nu pot sa-l iau si pe asta. Continui...\n");
				close(client);
			}
			else
			{
				/* nu stiu daca si aici trebuie sa setez non-blocant, dar... na */
				if ((arg = fcntl(client, F_GETFL, NULL)) < 0)
					die("Eroare la citirea flag-urilor clientului.");
				if (fcntl(client, F_SETFL, arg | O_NONBLOCK) == -1)
					die("Eroare la setarea flag-urilor clientului.");

				/* adaug clientul in primul loc liber si maresc numar de clienti */
				for (i = 0; i < clienti_nr + 1; i++)
					if (clienti[i] == -1)
					{
						clienti[i] = client;
						break;
					}
				clienti_nr++;

				/* trebuie sa-l anunt pe cel care foloseste functia asta ca s-a conectat cineva */
				sa_conectat(clienti_nr - 1);
			}
		}

		/* acum trebuie sa-l citesc pe fiecare in parte */
		for (i = 0; i < CLIENTI_SIZE; i++)
		{
			#define MESAJ_SIZE 128
			char mesaj[MESAJ_SIZE];
			int n;

			if (clienti[i] == -1) continue;
				
			n = read(clienti[i], mesaj, MESAJ_SIZE - 1);

			if (n == -1)
			{
				if (errno != EAGAIN) perror("read()");
			}
			else if (n == 0)
			{
				/* trebuie sa-l anunt pe cel care foloseste functia asta ca s-a deconectat cineva */
				a_plecat(i);
				close(clienti[i]);
				clienti[i] = -1;
				clienti_nr--;
				break; /* am modificat numarul de clienti, deci trebuie sa o iau de la inceput */
			}
			else
			{
				/* trebuie sa-l anunt pe cel care foloseste functia asta ca cineva i-a trimis un mesaj */
				mesaj_de_la(i, mesaj, n);
			}
		}

		/* noapte buna... pentru 50 de milisecunde */
		usleep(500000);
	}
}
/* Nu-i foarte frumos ce am facut aici, dar incerc asa sa vad ce risc. */
void scrie_catre(int cine, char* mesaj, int cat)
{
	int i;

	/* scrie la toti */
	if (cine == -1)
	{
		for (i = 0; i < CLIENTI_SIZE; i++)
			if (clienti[i] != -1)
				scrie_catre(i, mesaj, cat);
	}
	else
	{
		for (i = 0; i < 5; i++)
		{
			if (write(clienti[cine], mesaj, cat) > 0)
				return;
			usleep(10000 * (i+1));
		}
		fprintf(stderr, "Eroare la scrierea uni mesaj catre clientul %d.\n", cine);
	}
}
