#ifndef _USER_H_
#define _USER_H_

#include "bullet_list.h"

#define MOVE_RIGHT  1
#define MOVE_LEFT   -1

// временный константы, потом будут перенесены в другое место
#define TERMINAL_WIDTH 80
#define TERMINAL_HEIGHT 24

struct player {
  struct point *coord;
  struct bullet_list *list;
  char *image;
  unsigned int hp;
  unsigned int points;
  unsigned int damage;
  int id;
};

struct player *user_init(struct point *);

void user_fire(struct player *);

void user_move(struct player *, int);

void user_dest(struct player *);

#endif //_USER_H_
