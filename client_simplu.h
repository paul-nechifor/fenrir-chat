#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define die(format, ...) { fprintf(stderr, format "\n", ## __VA_ARGS__); exit(EXIT_FAILURE); }

void creare_client(char* adresa, int port, int* sd);
