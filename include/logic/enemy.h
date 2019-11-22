#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "bullet.h"

enum enemy_types
{
    WEAK,
    MEDIUM,
    HARD,
    RARE
};

struct enemy
{
    struct bullet * bullet;
    char * image;
    struct point * coord;
    unsigned int hp;
    unsigned int type;
    unsigned int points;
    unsigned int damage;
};

struct enemy * enemy_init(struct point *, unsigned int);

struct bullet * enemy_fire(struct enemy *);

void enemy_move(struct enemy *, unsigned int);

void enemy_dest(struct enemy *);

#endif // _ENEMY_H_
