#include <signal.h>
#include <ncurses.h>
#include "client_simplu.h"

char total[23][81] = {
	"",
	"",
	"",
	"",
	"",
	"",
" ______________________________________________________________________________ ",
"/       |\\               ___               _          ___ _           _        \\",
"|      |V \\_            / __\\__ _ __  _ __(_)_ __    / __\\ |__   __ _| |_      |",
"|      |  ' \\          / _\\/ _ \\ '_ \\| '__| | '__|  / /  | '_ \\ / _` | __|    N|",
"|      )   ,_\\        / / |  __/ | | | |  | | |    / /___| | | | (_| | |_     E|",
"|     /    |          \\/   \\___|_| |_|_|  |_|_|    \\____/|_| |_|\\__,_|\\__|  P C|",
"|    /      \\                                                               A H|",
"|    |       \\         Fenrir is the  father of the wolves Skoll and Hati   U I|",
"|     \\       \\        Hrodhvitnisson, is a son of Loki,  and is foretold   L F|",
"|      |       \\       to kill the god Odin during the events of Ragnarok,    O|",
"|      | |      |      but will  in turn be killed  by Odin's son Vidharr.    R|",
"\\______/ |\\     /______________________________________________________________/",
	"                                 Ctrl+C pentru a iesi.",
	"",
	"",
	"",
	""
};

int sd, buf_plin = 0;

void sigint(int signo)
{
	close(sd);
	endwin();
	exit(EXIT_SUCCESS);
}
void mai_citeste()
{
	#define MBUF 4096
	char buf[MBUF];
	int i, j, n;

	n = read(sd, buf + buf_plin, MBUF);
	if (n == -1 && errno != EAGAIN) die("Alta problema!!!");
	if (n > 0)
	{
		int este = 0;
		for (i = 0; i < buf_plin + n; i++)
			if (buf[i] == '\0')
			{
				este = 1;
				break;
			}
		if (este)
		{
			adauga_linie(buf);
			for (j = i + 1; j < buf_plin + n; j++)
				buf[j - i - 1] = buf[j];
			buf_plin = buf_plin + n - i - 1;
		}
		else
		{
			buf_plin += n;
		}
	}
}
void trimite_mesajul(char* mesaj)
{
	int n, i, len = strlen(mesaj) + 1;
	for (i = 0; i < 5; i++)
	{
		n = write(sd, mesaj, len);
		if (n > 0) return;
		if (n == -1 && errno != EAGAIN) die("Am o problema!");
	}
	die("Nu am reusit sa scriu mesajul.");
}
void adauga_linie(char* mesaj)
{
	int i, j, l;

	for (i = 0; i < 22; i++)
		strcpy(total[i], total[i+1]);
	strcpy(total[22], mesaj);

	for (i = 0; i < 23; i++)
	{
		move(i, 0);
		printw(total[i]);
		l = 80 - strlen(total[i]);
		for (j = 0; j < l; j++) addch(' ');
	}
	refresh();
}
int main(int argc, char* argv[])
{
	int x, y, start, marime;
	#define LINIE_SIZE 81
	char c, linie[LINIE_SIZE];

	if (argc != 2) die("Nu ai spus cum te chiama. Trebuie sa apelezi %s 'Numele tau'", argv[0]);

	signal(SIGINT, sigint);
	creare_client("127.0.0.1", 6666, &sd);

	initscr();
	start_color();
	init_pair(1, COLOR_GREEN, COLOR_BLACK);
	attrset(COLOR_PAIR(1));

	getmaxyx(stdscr, y, x);
	if (x < 80)
	{
		endwin();
		die("Trebuie sa ai mai mult de 80 de coloane. Acum ai %d. Mareste fereastra.", x);
	}
	adauga_linie("");

	for (;;)
	{
		strcpy(linie, argv[1]);
		strcat(linie, ": ");
		start = marime = strlen(linie);
		move(23, 0);
		printw(linie);
		refresh();
		timeout(100);
		noecho();

		for (;;)
		{
			mai_citeste();
			move(23, marime);
			c = getch();
			if (marime >= 80) c = '\n';
			if (c == ERR) continue;
			else if (c == '\n')
			{
				int i;
				if (marime > start)
				{
					trimite_mesajul(linie);
					move(23, 0);
					for (i = 0; i < 80; i++) addch(' ');
					break;
				}
			}
			else if (c == 127)
			{
				if (marime > start)
				{
					linie[--marime] = '\0';
					move(23, marime);
					addch(' ');
					move(23, marime);
				}
			}
			else
			{
				addch(c);
				linie[marime++] = c;
				linie[marime] = '\0';
			}
		}
	}

	endwin();
}
