#ifndef _BULLET_H_
#define _BULLET_H_

#include "game.h"

struct bullet {
  char *image;
  struct point *coord;
  unsigned int id;
  unsigned int tag;
};

struct bullet *bullet_init(struct point *, int);

bool bullet_move(struct bullet *);

void bullet_dest(struct bullet *);

#endif // _BULLET_H_