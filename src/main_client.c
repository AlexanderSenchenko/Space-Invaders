#include <stdio.h>
#include <curses.h>
#include "../include/graphics/menu.h"
#include "../include/network/client.h"

int main(int argc, char **argv)
{
  char *enemy_model = "###";
  char *player_model = "_/^\\_";
  char *bullet_model = "!";
  struct point enemy_position;
  struct point player_position;
  struct point bullet_position;

  ncurses_init();

  int ret_act = menu_do();

  if (ret_act == STATUS_PLAY) {
    /*
     * экран ожидания(подключение к серверу)
     */

    draw_waiting_for_connection();

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
        draw_waiting_for_player();
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
    /*
    Функция draw_game_field не полная. По идее должна принимать позиции врага
    и игроков, а также моделки для отрисовки
    */
    WINDOW *game_field = draw_game_field();

    // времено задал координаты игрока, пока не получили их с сервера
    player_position.x = TERMINAL_HEIGHT - 1;
    player_position.y = 12;

    while (1) {
      //читаем с клавы
      int status = get_player_action_from_keyboard(game_field,
                   &player_position,
                   &bullet_position,
                   player_model,
                   bullet_model);

      if (status == STATUS_EXIT) {
        break;
      }

      //смотрим (неблокирующе) есть ли пакет от сервера
    }

    delwin(game_field);
  }

  endwin();

  return 0;
}
