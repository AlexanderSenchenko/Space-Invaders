#include <string.h>
#include "../../include/logic/user.h"

extern char *player_models[];

struct player *user_init(struct point *point)
{
  struct player *user = (struct player *)malloc(sizeof(struct player));

  user->coord = point;
  user->list = NULL;
  user->hp = 3;
  user->damage = 1;
  user->id = -1;
  user->image = player_models[0];

  return user;
}

void user_fire(struct player *user)
{
  static unsigned int id = 1;

  int shift = strlen(user->image) / 2;

  struct point *tmp = (struct point *)malloc(sizeof(struct point));
  tmp->x = user->coord->y + shift;
  tmp->y = user->coord->x - 1;

  struct bullet *bull = bullet_init(tmp, USER);

  bull->id = id++;

  Push(&user->list, bull);
}

void user_move(struct player *user, int where)
{
  int new_coord_y = user->coord->y;
  int len_user_model = strlen(user->image);

  switch (where) {
  case MOVE_RIGHT:
    new_coord_y += 1;

    if (new_coord_y <= TERMINAL_WIDTH - len_user_model)
      user->coord->y = new_coord_y;

    break;

  case MOVE_LEFT:
    new_coord_y -= 1;

    if (new_coord_y >= 0)
      user->coord->y = new_coord_y;

    break;

  default:
    break;
  }
}

void user_dest(struct player *user)
{
  if (user) {
    if (user->coord)
      free(user->coord);

    free(user);
  }
}
