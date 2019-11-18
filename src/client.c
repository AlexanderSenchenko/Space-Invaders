#include <stdio.h>
#include <curses.h>

int main(int argc, const char **argv)
{
	initscr();
	cbreak();
	curs_set(FALSE);
	start_color();
	noecho();
	refresh();
	
	int flag = 1;
	while(flag)
	{
		keypad(stdscr, TRUE);
		int ch = getch();
		switch(ch)
		{
			case 'q': flag = 0; break;		
		}
	}
	
	endwin();
	return 0;
}
