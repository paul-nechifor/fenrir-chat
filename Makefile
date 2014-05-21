all: clean client server sfarsit linii
client: client.o client_simplu.o
	gcc -o client client.o client_simplu.o -lcurses
server: server.o server_simplu.o
	gcc -o server server.o server_simplu.o
client.o:
	gcc -Wall -c client.c -lcurses
server.o:
	gcc -Wall -c server.c
client_simplu.o:
	gcc -Wall -c client_simplu.c
server_simplu.o:
	gcc -Wall -c server_simplu.c
sfarsit:
	rm -fr *.o
clean:
	rm -fr *.o client server
linii:
	clear
	wc *.c Makefile
