#include "menu.h"

void ncurses_init()
{
	initscr();
	cbreak();
	curs_set(0);
	start_color();
	noecho();
	refresh();
	init_pair(1, COLOR_YELLOW, COLOR_BLUE);
}

void menu_init(Menu* menu)
{
	menu->menu_wnd = newwin(NUM_MENU_ITEMS, 10, TERMINAL_HEIGHT / 2, TERMINAL_WIDTH / 2);
	for(int i = 0; i < NUM_MENU_ITEMS; i++)
	{
		menu->menu_items[i] = derwin(menu->menu_wnd, 1, 10, i, 0);
	}
	wprintw(menu->menu_items[0], "Play");
	wprintw(menu->menu_items[1], "Exit");
	
	wbkgd(menu->menu_wnd, COLOR_PAIR(1));
	wbkgd(menu->menu_items[0], COLOR_PAIR(1) | A_BOLD);
	wrefresh(menu->menu_wnd);
}

void menu_destroy(Menu* menu)
{
	for(int i = 0; i < NUM_MENU_ITEMS; i++)
	{
		delwin(menu->menu_items[i]);
	}
	delwin(menu->menu_wnd);
}
