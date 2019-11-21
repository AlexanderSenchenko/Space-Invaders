#include "../../include/logic/bullet.h"

bullet_t * bullet_init(point_t * point, int tag)
{
    bullet_t * shot = (bullet_t *)malloc(sizeof(bullet_t));

    shot->coord = point;
    shot->tag = tag;

    switch(tag)
    {
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

bool bullet_move(bullet_t * shot)
{   

    shot->coord->y -= 2;
    if(shot->coord->y <= 0)
    {
        bullet_dest(shot);
        return false;
    }

    //else if( Попал )
        //dest(shot);
        //
    else
    return true;
    
    
}

void bullet_dest(bullet_t * shot)
{
    free(shot);
}
