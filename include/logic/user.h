#ifndef _USER_H_
#define _USER_H_

#include "bullet.h"

typedef struct player
{
    bullet_t * bullet;
    char * image;
    point_t * coord;
    unsigned int hp;
    unsigned int points;
    unsigned int damage;
}player_t;


player_t * user_init(point_t *);

bullet_t * user_fire(player_t *);

void user_move(player_t *, unsigned int);

void user_dest(player_t *);

#endif //_USER_H_
