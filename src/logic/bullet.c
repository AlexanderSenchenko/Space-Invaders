#include "../../include/logic/bullet.h"

extern char *bullet_models[];

struct bullet *bullet_init(struct point *point, int tag)
{
  struct bullet *shot = (struct bullet *)malloc(sizeof(struct bullet));

  shot->id = 0;
  shot->coord = point;
  shot->tag = tag;

  switch (tag) {
  case USER:
    shot->image = bullet_models[0];
    break;

  case ENEMY:
    shot->image = bullet_models[1];
    break;

    //default:
    //error tag
  }

  return shot;
}

bool bullet_move(struct bullet *shot)
{

  shot->coord->y -= 2;

  if (shot->coord->y <= 0) {
    bullet_dest(shot);
    return false;
  }

  //else if( Попал )
  //dest(shot);
  //
  else
    return true;
}

void bullet_dest(struct bullet *shot)
{
  free(shot);
}