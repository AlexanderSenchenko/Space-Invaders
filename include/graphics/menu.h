#ifndef _MENU_H_
#define _MENU_H_

#include <curses.h>
#include "../../include/logic/game.h"

#define TERMINAL_WIDTH 80
#define TERMINAL_HEIGHT 24
#define NUM_MENU_ITEMS 2
#define STATUS_PLAY 0
#define STATUS_EXIT 1

struct Menu {
  WINDOW *menu_wnd;
  WINDOW *menu_items[NUM_MENU_ITEMS];
  int current_idx;
};

void sig_winch(int signo);
void check_terminal_size();

void ncurses_init();
void menu_init(struct Menu *menu);
void menu_destroy(struct Menu *menu);

void menu_go_up(struct Menu *menu);
void menu_go_down(struct Menu *menu);
int menu_move(struct Menu *menu);

int menu_act_on_item(struct Menu *menu);

int menu_do();

void draw_waiting_for_connection();
void draw_waiting_for_player();

WINDOW *draw_game_field(const struct game *game);

void erase_entity(WINDOW *game_field, const struct point *entity_positon,
                  const char *entity_model);
void draw_entity(WINDOW *game_field, const struct point *entity_positon,
                 const char *entity_model);

int get_player_action_from_keyboard(WINDOW *game_field,
                                    struct game *game,
                                    struct point *bullet_positon,
                                    const char *bullet_model);

void refresh_plaer(WINDOW *game_field, struct player *plr_opd,
                   struct player *plr_new);
int server_listening(WINDOW *game_field, struct game *game,
                     struct player **plr2);

#endif // _MENU_H_