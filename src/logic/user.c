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

void user_move(struct point *positon, int where)
{
  int new_coord_y = positon->y;

  // длину модельки стоит как-то вычислять я не задавать константно
  int len_user_model = 5;

  switch (where) {
  case MOVE_RIGHT:
    new_coord_y += 1;

    if (new_coord_y <= TERMINAL_WIDTH - len_user_model)
      positon->y = new_coord_y;

    break;

  case MOVE_LEFT:
    new_coord_y -= 1;

    if (new_coord_y >= 0)
      positon->y = new_coord_y;

    break;

  default:
    break;
  }
}

void user_dest(struct player *user)
{
  free(user);
}
