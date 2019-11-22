#ifndef _GAME_H_
#define _GAME_H_

#include <stdlib.h>
#include <stdbool.h>

#include "user.h"
#include "enemy.h"

#define Fire(X) _Generic((X), \
    player_t *: user_fire, \
    enemy_t *: enemy_fire \
)(X)

#define Dest(X) _Generic((X), \
    player_t *: user_dest, \
    enemy_t *: enemy_dest, \
    bullet_t *: bullet_dest \
)(X)

#define Move(X, Y) _Generic((X), \
    enemy_t *:   _Generic((Y), \
        unsigned int:  enemy_move, \
    ), \
    player_t *: _Generic((Y), \
        unsigned int:  user_move, \
    ) \
)(X, Y)

enum TAGS {
  USER,
  ENEMY
};

struct point {
  int x;
  int y;
};

struct game {
  struct player **users;
  struct enemy **aliens;
  unsigned int score;
};

struct game *game_init(void);

#endif // _GAME_H_
