#include <termios.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/ioctl.h>
#include "../../include/graphics/menu.h"

void sig_winch(int signo)
{

}

void check_terminal_size()
{
  struct winsize size;
  ioctl(fileno(stdout), TIOCGWINSZ, (char *) &size);

  if (size.ws_col != TERMINAL_WIDTH || size.ws_row != TERMINAL_HEIGHT) {
    printf("Terminal has improper width/height!\n");
    printf("Recommended sizes are: width - %d  height - %d\n",
           TERMINAL_WIDTH, TERMINAL_HEIGHT);
    exit(EXIT_FAILURE);
  }
}

void ncurses_init()
{
  // check_terminal_size();

  initscr();
  signal(SIGWINCH, sig_winch);
  cbreak();
  curs_set(0);
  start_color();
  noecho();
  refresh();
  init_pair(1, COLOR_YELLOW, COLOR_BLUE);
  init_pair(2, COLOR_YELLOW, COLOR_CYAN);
  keypad(stdscr, 1);
  nodelay(stdscr, 1);
  bkgd(COLOR_PAIR(2));
  refresh();
}

void menu_init(Menu *menu)
{
  const int menu_box_offset = 2;
  int menu_width = 4 + menu_box_offset;
  int menu_height = NUM_MENU_ITEMS + menu_box_offset;
  int menu_ncurses_y = TERMINAL_HEIGHT / 2 - menu_height / 2;
  int menu_ncurses_x = TERMINAL_WIDTH / 2 - menu_width / 2;

  menu->menu_wnd = newwin(menu_height, menu_width, menu_ncurses_y,
                          menu_ncurses_x);

  int menu_item_width = menu_width - menu_box_offset;

  for (int i = 0; i < NUM_MENU_ITEMS; i++) {
    menu->menu_items[i] = derwin(menu->menu_wnd, 1, menu_item_width,
                                 i + menu_box_offset / 2,
                                 0 + menu_box_offset / 2);
  }

  wprintw(menu->menu_items[0], "Play");
  wprintw(menu->menu_items[1], "Exit");

  wbkgd(menu->menu_wnd, COLOR_PAIR(1));
  menu->current_idx = 0;
  wbkgd(menu->menu_items[menu->current_idx], COLOR_PAIR(1) | A_BOLD);

  box(menu->menu_wnd, '|', '-');
  wrefresh(menu->menu_wnd);
}

void menu_destroy(Menu *menu)
{
  for (int i = 0; i < NUM_MENU_ITEMS; i++) {
    delwin(menu->menu_items[i]);
  }

  delwin(menu->menu_wnd);
}

void menu_go_down(Menu *menu)
{
  if (menu->current_idx + 1 < NUM_MENU_ITEMS) {
    wbkgd(menu->menu_items[menu->current_idx], COLOR_PAIR(1));
    wrefresh(menu->menu_items[menu->current_idx]);
    wbkgd(menu->menu_items[++menu->current_idx], COLOR_PAIR(1) | A_BOLD);
    wrefresh(menu->menu_items[menu->current_idx]);
  }
}

void menu_go_up(Menu *menu)
{
  if (menu->current_idx - 1 >= 0) {
    wbkgd(menu->menu_items[menu->current_idx], COLOR_PAIR(1));
    wrefresh(menu->menu_items[menu->current_idx]);
    wbkgd(menu->menu_items[--menu->current_idx], COLOR_PAIR(1) | A_BOLD);
    wrefresh(menu->menu_items[menu->current_idx]);
  }
}

int menu_move(Menu *menu)
{

  while (1) {
    int ch = getch();

    switch (ch) {
    case 'q':
      return -1;

    case KEY_DOWN:
      menu_go_down(menu);
      break;

    case KEY_UP:
      menu_go_up(menu);
      break;

    case '\n':
      if (menu_act_on_item(menu))
        return 0;
      else
        return -1;

    case ERR:
      break;

    default:
      break;
    }
  }
}

int menu_act_on_item(Menu *menu)
{
  int status = 0;

  switch (menu->current_idx) {
  case 0:
    status = 1;
    break;

  case 1:
    status = -1;
    break;

  default:
    break;
  }

  return status;
}