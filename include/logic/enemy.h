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

typedef struct enemy
{
    bullet_t * bullet;
    char * image;
    point_t * coord;
    unsigned int hp;
    unsigned int type;
    unsigned int points;
    unsigned int damage;
}enemy_t;

enemy_t * enemy_init(point_t *, unsigned int);

bullet_t * enemy_fire(enemy_t *);

void enemy_move(enemy_t *, unsigned int);

void enemy_dest(enemy_t *);

#endif // _ENEMY_H_
