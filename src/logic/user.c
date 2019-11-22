#include "../../include/logic/user.h"

extern char * player_models[];

struct player * user_init(struct point * point)
{
    struct player * user = (struct player *)malloc(sizeof(struct player));

    user->coord = point;
    user->bullet = NULL;
    user->hp = 3;
    user->damage = 1;
    user->image = player_models[0];

    return user;
}

struct bullet * user_fire(struct player * user)
{
    user->bullet = bullet_init(user->coord, USER);
}

void user_move(struct player * user, unsigned int where)
{
    switch(where)
    {
        case 0:
            user->coord->x -=2;
            break;
        
        case 1:
            user->coord->x +=2;
            break;
    }

}

void user_dest(struct player * user)
{
    free(user);
}
