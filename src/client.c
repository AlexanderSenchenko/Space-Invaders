#include <stdio.h>
#include <curses.h>
#include "menu.h"

#define TERMINAL_HEIGHT 24 
#define TERMINAL_WIDTH 80

int main(int argc, const char **argv)
{
	ncurses_init();
	Menu main_menu;
	menu_init(&main_menu);
		
	/*
	int flag = 1;
	while(flag)
	{
		keypad(stdscr, TRUE);
		int ch = getch();
		switch(ch)
		{
			case 'q': flag = 0; break;		
		}
	}*/
	getch();
	menu_destroy(&main_menu);
	endwin();
	return 0;
}


