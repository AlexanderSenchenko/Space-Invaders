#include "../../include/logic/user.h"

extern char *player_models[];

struct player *user_init(struct point *point)
{
  struct player *user = (struct player *)malloc(sizeof(struct player));

  user->coord = point;
  user->bullet = NULL;
  user->hp = 3;
  user->damage = 1;
  user->image = player_models[0];

  return user;
}

void user_fire(struct player *user)
{
  user->bullet = bullet_init(user->coord, USER);
}

// отслеживание дейсвие с помощью curses
// недолжно быть логике
/*
void user_move(struct player *user, int where)
{
  switch (where) {
    case 'a':
      user->coord->x -= 2;// поставить сколько надо
      break;

    case 'd':
      user->coord->x += 2;//
      break;

    case KEY_LEFT:
      user->coord->x -= 2;//
      break;

    case KEY_RIGHT:
      user->coord->x += 2;//
      break;

    case ' ':
      user_fire(user);
      break;

  }

}
*/

void user_dest(struct player *user)
{
  free(user);
}
