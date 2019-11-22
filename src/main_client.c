#include <stdio.h>
#include <curses.h>
#include "../include/graphics/menu.h"
#include "../include/logic/game.h"

int main(int argc, char **argv)
{
  struct game *game = NULL;

  ncurses_init();

  Menu main_menu;
  menu_init(&main_menu);

  menu_move(&main_menu, argc, argv);
  game = game_init();
  // draw(game);

  menu_destroy(&main_menu);
  endwin();

  return 0;
}