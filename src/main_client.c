#include <stdio.h>
#include <curses.h>
#include "../include/graphics/menu.h"
#include "../include/logic/game.h"
#include "../include/network/client.h"

int main(int argc, char **argv)
{
  char *enemy_model = "###";
  char *player_model = "###";
  char *bullet_model = "###";
  struct point enemy_position;
  struct point player_position;
  struct point bullet_position;

  struct game *game = NULL;
  ncurses_init();

  // game = game_init();

  int ret_act = menu_do();

  if (ret_act == STATUS_PLAY) {
    /*
     * экран ожидания(подключение к серверу)
     */

    draw_waiting_for_connection();

    init_client(argc, argv);

    int stat_connect_to_serv = 0;


    stat_connect_to_serv = reception();
    draw_waiting_for_player();

    if (wait_start_of_game() == 2) {
      /*
       * отрисовать поле игры
       */
      /*
      Функция draw_game_field не полная. По идее должна принимать позиции врага
      и игроков, а также моделки для отрисовки
      */
      WINDOW *game_field = draw_game_field();

      while (1) {
        //читаем с клавы
        if (STATUS_EXIT == get_player_action_from_keyboard(game_field,
            &player_position, &bullet_position, player_model, bullet_model)) {
          break;
        }

        //смотрим (неблокирующе) есть ли пакет от сервера
      }

      delwin(game_field);
    }

  }

  endwin();

  return 0;
}
