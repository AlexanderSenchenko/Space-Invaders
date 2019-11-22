#ifndef _USER_H_
#define _USER_H_

#include "bullet.h"

struct player
{
    struct bullet * bullet;
    char * image;
    struct point * coord;
    unsigned int hp;
    unsigned int points;
    unsigned int damage;
};

struct player * user_init(struct point *);

struct bullet * user_fire(struct player *);

void user_move(struct player *, unsigned int);

void user_dest(struct player *);

#endif //_USER_H_
