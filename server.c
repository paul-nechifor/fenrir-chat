#include "server_simplu.h"

void sa_conectat(int cine)
{
	printf("S-a conectat clientul %d.\n", cine);
}
void a_plecat(int cine)
{
	printf("A plecat clientul %d.\n", cine);
}
void mesaj_de_la(int cine, char* mesaj, int cat)
{
	/* Nu fac nimic. Doar dau la ceilalti. */
	printf("Am primit un mesaj de la clientul %d.\n", cine);
	scrie_catre(-1, mesaj, cat);
}

int main(void)
{
	creare_server(6666);
	exit(EXIT_SUCCESS);
}
