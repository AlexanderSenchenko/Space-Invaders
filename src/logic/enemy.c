#include "../../include/logic/enemy.h"

extern char *enemy_models[];

struct enemy *enemy_init(struct point *point, unsigned int type)
{
  struct enemy *alien = (struct enemy *)malloc(sizeof(struct enemy));

  alien->coord = point;
  alien->bullet = NULL;
  alien->type = type;

  switch (type) {
  case WEAK:
    alien->hp = 1;
    alien->points = 5;
    alien->damage = 1;
    alien->image = enemy_models[0];
    break;

  case MEDIUM:
    alien->hp = 2;
    alien->points = 10;
    alien->damage = 2;
    alien->image = enemy_models[1];
    break;

  case HARD:
    alien->hp = 3;
    alien->points = 20;
    alien->damage = 3;
    alien->image = enemy_models[2];
    break;

  case RARE:
    alien->hp = 1;
    alien->points = 50;
    alien->damage = 5;
    alien->image = enemy_models[3];
    break;

    //default:
    //error type
  }

  return alien;
}

struct bullet *enemy_fire(struct enemy *alien)
{
  alien->bullet = bullet_init(alien->coord, ENEMY);
}

void enemy_move(struct enemy *alien, unsigned int where)
{
  switch (where) {
  case 0:
    alien->coord->x -= 2;
    break;

  case 1:
    alien->coord->x += 2;
    break;

  case 2:
    alien->coord->y += 2;
    break;
  }
}

void enemy_dest(struct enemy *alien)
{
  free(alien);
}
