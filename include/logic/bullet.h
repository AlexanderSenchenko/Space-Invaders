#ifndef _BULLET_H_
#define _BULLET_H_

#include "game.h"


typedef struct bullet
{
    char * image;
    int x;
    int y;
    unsigned int tag;

}bullet_t;

bullet_t * bullet_init(int, int, int);

bool bullet_move(bullet_t *);

void bullet_dest(bullet_t *);

#endif // _BULLET_H_