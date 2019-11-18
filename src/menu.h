#include <curses.h>

#define TERMINAL_HEIGHT 24 
#define TERMINAL_WIDTH 80
#define NUM_MENU_ITEMS 2

struct Menu_
{
	WINDOW* menu_wnd;
	WINDOW* menu_items[NUM_MENU_ITEMS];
};
typedef struct Menu_ Menu;

void ncurses_init();
void menu_init(Menu* menu);
void menu_destroy(Menu* menu);
