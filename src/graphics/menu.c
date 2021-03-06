#include <termios.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <string.h>
#include "../../include/graphics/menu.h"
#include "../../include/network/client.h"

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
  init_pair(3, COLOR_WHITE, COLOR_BLACK);
  keypad(stdscr, 1);
  nodelay(stdscr, 1);
  bkgd(COLOR_PAIR(2));
  refresh();
}

void menu_init(struct Menu *menu)
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

void menu_destroy(struct Menu *menu)
{
  for (int i = 0; i < NUM_MENU_ITEMS; i++) {
    delwin(menu->menu_items[i]);
  }

  delwin(menu->menu_wnd);
}

void menu_go_down(struct Menu *menu)
{
  if (menu->current_idx + 1 < NUM_MENU_ITEMS) {
    wbkgd(menu->menu_items[menu->current_idx], COLOR_PAIR(1));
    wrefresh(menu->menu_items[menu->current_idx]);
    wbkgd(menu->menu_items[++menu->current_idx], COLOR_PAIR(1) | A_BOLD);
    wrefresh(menu->menu_items[menu->current_idx]);
  }
}

void menu_go_up(struct Menu *menu)
{
  if (menu->current_idx - 1 >= 0) {
    wbkgd(menu->menu_items[menu->current_idx], COLOR_PAIR(1));
    wrefresh(menu->menu_items[menu->current_idx]);
    wbkgd(menu->menu_items[--menu->current_idx], COLOR_PAIR(1) | A_BOLD);
    wrefresh(menu->menu_items[menu->current_idx]);
  }
}

int menu_move(struct Menu *menu)
{

  while (1) {
    int ch = getch();

    switch (ch) {
    case 'q':
      return STATUS_EXIT;

    case KEY_DOWN:
      menu_go_down(menu);
      break;

    case KEY_UP:
      menu_go_up(menu);
      break;

    case '\n':
      return (menu_act_on_item(menu));

    case ERR:
      break;

    default:
      break;
    }
  }
}

int menu_act_on_item(struct Menu *menu)
{
  int status = -1;

  switch (menu->current_idx) {
  case 0:
    status = STATUS_PLAY;
    break;

  case 1:
    status = STATUS_EXIT;
    break;

  default:
    break;
  }

  return status;
}

int menu_do()
{
  struct Menu main_menu;
  menu_init(&main_menu);
  int player_choice = menu_move(&main_menu);
  menu_destroy(&main_menu);

  return player_choice;
}

void draw_waiting_for_connection()
{
  erase();
  bkgd(COLOR_PAIR(3));
  move(TERMINAL_HEIGHT - 1, 0);
  printw("Waiting for server response");
  refresh();
}

void draw_waiting_for_player()
{
  erase();
  move(TERMINAL_HEIGHT - 1, 0);
  printw("Waiting for second player to connect");
  refresh();
}

WINDOW *draw_game_field(const struct game *game)
{
  erase();
  WINDOW *wnd = newwin(TERMINAL_HEIGHT, TERMINAL_WIDTH, 0, 0);
  wbkgd(wnd, COLOR_PAIR(3));
  refresh();
  wrefresh(wnd);

  draw_entity(wnd, game->user->coord, game->user->image);

  // отправка серверу начальных координат
  send_message(STS_MOVE, game->user->id, game->user->coord,
               sizeof(struct point));

  return wnd;
}

void erase_entity(WINDOW *game_field, const struct point *entity_positon,
                  const char *entity_model)
{
  int len = strlen(entity_model);
  wmove(game_field, entity_positon->x, entity_positon->y);

  for (int i = 0; i < len; i++) {
    waddch(game_field, ' ');
  }

  wrefresh(game_field);
}

void draw_entity(WINDOW *game_field, const struct point *entity_positon,
                 const char *entity_model)
{
  int len = strlen(entity_model);
  wmove(game_field, entity_positon->x, entity_positon->y);

  for (int i = 0; i < len; i++) {
    waddch(game_field, entity_model[i]);
  }

  wrefresh(game_field);
}

int get_player_action_from_keyboard(WINDOW *game_field,
                                    struct game *game,
                                    struct point *bullet_positon,
                                    const char *bullet_model)
{
  int ch = getch();

  switch (ch) {
  case 'q':
    send_message(STS_END, game->user->id, NULL, 0);
    return STATUS_EXIT;
    break;

  case KEY_LEFT: // игрок сдвинулся влево
    erase_entity(game_field, game->user->coord, game->user->image);
    refresh_plaer(game_field, game->second_user, game->second_user);

    // обновление координат
    user_move(game->user, MOVE_LEFT);

    draw_entity(game_field, game->user->coord, game->user->image);

    // отсылка инфы серверу
    send_message(STS_MOVE, game->user->id, game->user->coord,
                 sizeof(struct point));

    break;

  case KEY_RIGHT: // игрок сдвинулся вправо
    erase_entity(game_field, game->user->coord, game->user->image);
    refresh_plaer(game_field, game->second_user, game->second_user);

    // обновление координат
    user_move(game->user, MOVE_RIGHT);

    draw_entity(game_field, game->user->coord, game->user->image);

    // отсылка инфы серверу
    send_message(STS_MOVE, game->user->id, (void *) game->user->coord,
                 sizeof(struct point));
    break;

  case ' ': // игрок выстрелил

    // user_fire(game->user);

    draw_entity(game_field, bullet_positon, bullet_model);

  // отсылка инфы серверу
  default:
    break;
  }

  return STATUS_PLAY;
}

void refresh_plaer(WINDOW *game_field, struct player *plr_opd,
                   struct player *plr_new)
{
  erase_entity(game_field, plr_opd->coord, plr_opd->image);
  draw_entity(game_field, plr_new->coord, plr_new->image);
}

/*
 * Прием информации в неблокирующем режиме
 * Пока реализована обновление координат второго пользователя
 *
 */
int server_listening(WINDOW *game_field, struct game *game,
                     struct player **plr2)
{
  int ret_recv;
  struct point *coord = calloc(1, sizeof(struct point));
  struct player *plr2_new = user_init(coord);

  ret_recv = recv_message_dontwait(plr2_new);

  if (ret_recv == STC_MOVE) {
    if (*plr2 == NULL) {
      *plr2 = calloc(1, sizeof(struct player));
      refresh_plaer(game_field, plr2_new, plr2_new);
    } else {
      refresh_plaer(game_field, *plr2, plr2_new);
      memcpy(game->second_user, plr2_new, sizeof(struct player));
    }

    memcpy(*plr2, plr2_new, sizeof(struct player));

    refresh_plaer(game_field, game->user, game->user);

    free(plr2_new);

    return STC_MOVE;
  } else if (ret_recv = STC_END) {
    return STC_END;
  }

  return 0;
}
