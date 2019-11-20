#ifndef _GAME_H_
#define _GAME_H_

#include <stdlib.h>
#include <stdbool.h>

#include "models.h"

enum TAGS
{
    USER,
    ENEMY
};
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

#endif // _GAME_H_