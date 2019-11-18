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
	
	
	int flag = 1;
	while(flag)
	{
		int ch = getch();
		switch(ch)
		{
			case 'q': flag = 0; break;
			case KEY_DOWN: menu_go_down(&main_menu); break;
			case KEY_UP: menu_go_up(&main_menu); break;
			default: break;	
		}
	}
	
	menu_destroy(&main_menu);
	endwin();
	return 0;
}

