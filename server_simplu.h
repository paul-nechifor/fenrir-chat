#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define die(format, ...) { fprintf(stderr, format "\n", ## __VA_ARGS__); exit(EXIT_FAILURE); }

/* daca cine == -1 inseamna scrie la toti */
void scrie_catre(int cine, char* mesaj, int cat);
extern void mesaj_de_la(int cine, char* mesaj, int cat);
extern void sa_conectat(int cine);
extern void a_plecat(int cine);
