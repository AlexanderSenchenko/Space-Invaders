#include "../../include/logic/game.h"

struct game *game_init(void)
{
  struct game *game = (struct game *)malloc(sizeof(struct game));

  struct point *user_coord = (struct point *)malloc(sizeof(struct point ));
  struct player *user = (struct player *)malloc(sizeof(struct player ));

  user_coord->x = 0; // ЗДЕСЬ ВСТАВИТЬ НАЧАЛЬНЫЕ КООРДЫ
  user_coord->y = 0 + 1; // ДЛЯ ИГРОКОВ

  user = user_init(user_coord);


  // инициализация для первого слоя мобов
  struct point **alien_coord = (struct point **)malloc(12 * sizeof(
                                 struct point *));
  struct enemy **aliens = (struct enemy **)malloc(12 * sizeof(struct enemy *));

  for (int i = 0; i < 4; i++) {
    alien_coord[i] = (struct point *)malloc(sizeof(struct point));

    alien_coord[i]->x = i; // ЗДЕСЬ ВСТАВИТЬ НАЧАЛЬНЫЕ КООРДЫ
    alien_coord[i]->y = i + 1; // ДЛЯ МОБОВ

    aliens[i] = enemy_init(alien_coord[i], WEAK);
  }

  // инициализация для второго слоя мобов
  for (int i = 4; i < 8; i++) {
    alien_coord[i] = (struct point *)malloc(sizeof(struct point));

    alien_coord[i]->x = i; // ЗДЕСЬ ВСТАВИТЬ НАЧАЛЬНЫЕ КООРДЫ
    alien_coord[i]->y = i + 1;// ДЛЯ МОБОВ

    aliens[i] = enemy_init(alien_coord[i], MEDIUM);
  }

  // инициализация для третьего слоя мобов
  for (int i = 8; i < 12; i++) {
    alien_coord[i] = (struct point *)malloc(sizeof(struct point));

    alien_coord[i]->x = i; // ЗДЕСЬ ВСТАВИТЬ НАЧАЛЬНЫЕ КООРДЫ
    alien_coord[i]->y = i + 1;// ДЛЯ МОБОВ

    aliens[i] = enemy_init(alien_coord[i], HARD);
  }

  game->aliens = aliens;
  game->user = user;
  game->score = 0;
}

// curses тут не нужен
/*
void game_user_move(struct game * game)
{
  int ch;
  while(ch = getch()) {
    switch(ch) {
    case 'a':
      user_move(game->user,ch);
      break;

    case 'd':
      user_move(game->user,ch);
      break;

    case KEY_LEFT:
      user_move(game->user,ch);
      break;

    case KEY_RIGHT:
      user_move(game->user,ch);
      break;

    case ' ':
      user_fire(game->user);
      break;
    }

  }
}

void game_process(struct game * game)
{
    game_user_move(game);

}
*/