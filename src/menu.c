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
	keypad(stdscr, 1);
	nodelay(stdscr, 1);
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
	menu->current_idx = 0;
	wbkgd(menu->menu_items[menu->current_idx], COLOR_PAIR(1) | A_BOLD);
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

void menu_go_down(Menu* menu)
{
	if(menu->current_idx + 1 < NUM_MENU_ITEMS)
	{
		wbkgd(menu->menu_items[menu->current_idx], COLOR_PAIR(1));
		wrefresh(menu->menu_items[menu->current_idx]);
		wbkgd(menu->menu_items[++menu->current_idx], COLOR_PAIR(1) | A_BOLD);
		wrefresh(menu->menu_items[menu->current_idx]);
	}
}

void menu_go_up(Menu* menu)
{
	if(menu->current_idx - 1 >= 0)
	{
		wbkgd(menu->menu_items[menu->current_idx], COLOR_PAIR(1));
		wrefresh(menu->menu_items[menu->current_idx]);
		wbkgd(menu->menu_items[--menu->current_idx], COLOR_PAIR(1) | A_BOLD);
		wrefresh(menu->menu_items[menu->current_idx]);
	}
}

int menu_act_on_item(Menu* menu)
{
	int close_program = 0;
	switch(menu->current_idx)
	{
		case 0: /* Тут должен быть вызов функции, отправляющей серверу предложение об игре */break;
		case 1: close_program = 1; break;
		default: break;
	}
	
	return close_program;
}
