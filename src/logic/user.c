#include "../../include/logic/user.h"

player_t * user_init(point_t * point)
{
    player_t * user = (player_t *)malloc(sizeof(player_t));

    user->coord = point;
    user->bullet = NULL;
    user->hp = 3;
    user->damage = 1;
    user->image = player_models[0];

    return user;
}

bullet_t * user_fire(player_t * user)
{
    user->bullet = bullet_init(user->coord, USER);
}

void user_move(player_t * user, unsigned int where)
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

void user_dest(player_t * user)
{
    free(user);
}
