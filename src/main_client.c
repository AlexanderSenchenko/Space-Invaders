#include <stdio.h>
#include <curses.h>
#include "../include/graphics/menu.h"
#include "../include/logic/game.h"
#include "../include/network/client.h"

int main(int argc, char **argv)
{
  struct game *game = NULL;
  ncurses_init();

  Menu main_menu;
  menu_init(&main_menu);

  // game = game_init();

  int ret_act = menu_move(&main_menu);
  menu_destroy(&main_menu);

  if (ret_act == 0) {
    /*
     * экран ожидания(подключение к серверу)
     */

    init_client(argc, argv);

    int stat_connect_to_serv = 0;

    /*
     * stat_connect_to_serv != 1 временно, пока, нет функционала
     * подключение второго клиента
     * stat_connect_to_serv != 2 в будущем
     */
    while (stat_connect_to_serv != 1) {
      stat_connect_to_serv = reception();

      // TODO: описать статусы подключения к серверу в define
      switch (stat_connect_to_serv) {
      case 1: // подключен к серверу, ожидание второго игрока
        /*
         * экран ожидания(подключение к второго игрока к серверу)
         */
        break;

      case 2: // сообщение о там, что игру можно начать
        /*
         * возможно, тут что-то происходит
         */
        break;

      default:
        /*
         * возможно сообщение об ошибке подключения к серверу
         */
        break;
      }
    }

    /*
     * отрисовать поле игры
     */
  }

  endwin();

  return 0;
}