#include "../../include/logic/user.h"

player_t * user_init(int x, int y)
{
    player_t * user = (player_t *)malloc(sizeof(player_t));

    user->x = x;
    user->y = y;
    user->bullet = NULL;
    user->hp = 3;
    user->damage = 1;
    user->image = player_models[0];

    return user;
}

bullet_t * user_fire(player_t * user)
{
    user->bullet = bullet_init(user->x, user->y, USER);
}

void user_move(player_t * user, unsigned int where)
{
    switch(where)
    {
        case 0:
            user->x -=2;
            break;
        
        case 1:
            user->x +=2;
            break;
    }

}

void user_dest(player_t * user)
{
    free(user);
}
