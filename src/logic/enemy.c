#include "../../include/logic/enemy.h"

enemy_t * enemy_init(int x, int y, unsigned int type)
{
    enemy_t * alien = (enemy_t *)malloc(sizeof(enemy_t));

    alien->x = x;
    alien->y = y;
    alien->bullet = NULL;
    alien->type = type;

    switch(type)
    {
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

bullet_t * enemy_fire(enemy_t * alien)
{
    alien->bullet = bullet_init(alien->x, alien->y, ENEMY);
}

void enemy_move(enemy_t * alien, unsigned int where)
{
    switch(where)
    {
        case 0:
            alien->x -=2;
            break;
        
        case 1:
            alien->x +=2;
            break;
        
        case 2:
            alien->y +=2;
            break;
    }

}

void enemy_dest(enemy_t *alien)
{
    free(alien);
}
