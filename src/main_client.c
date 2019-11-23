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

  if (0 == menu_move(&main_menu, argc, argv))
	{
    game = game_init();
    /*
    void draw(game * game)
    {
        рисуем пользователя
        game->user->image

        рисуем врагов 
        for(int i = 0; i < 12; i++)
        {
          сдвинуться на 
          game->aliens[i]->coord->x;
          game->aliens[i]->coord->y;
          нарисовать
          game->aliens[i]->image
        }

        game_process(game);
        
    }

    
  

  */
  }

  menu_destroy(&main_menu);
  endwin();

  return 0;
}
