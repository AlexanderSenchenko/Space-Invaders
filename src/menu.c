#include "menu.h"

void ncurses_init()
{
	initscr();
	cbreak();
	curs_set(0);
	start_color();
	noecho();
	refresh();
}
