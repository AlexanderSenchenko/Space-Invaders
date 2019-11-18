#include <stdio.h>
#include <curses.h>
#include "menu.h"

#define TERMINAL_HEIGHT 24 
#define TERMINAL_WIDTH 80

int main(int argc, const char **argv)
{
	ncurses_init();
	init_pair(1, COLOR_YELLOW, COLOR_BLUE);
	WINDOW* play_menu_item = newwin(1, 10, TERMINAL_HEIGHT / 2, TERMINAL_WIDTH / 2);
	WINDOW* exit_menu_item = newwin(1, 10, TERMINAL_HEIGHT / 2 + 1, TERMINAL_WIDTH / 2);
	wattron(play_menu_item, COLOR_PAIR(1));
	wattron(exit_menu_item, COLOR_PAIR(1));
	wprintw(play_menu_item, "Play");
	wprintw(exit_menu_item, "Exit");
	wbkgd(play_menu_item, COLOR_PAIR(1) | A_BOLD);
	wrefresh(play_menu_item);
	wrefresh(exit_menu_item);
	//refresh();
	
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
	delwin(play_menu_item);
	delwin(exit_menu_item);
	endwin();
	return 0;
}


