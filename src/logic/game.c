#include "../../include/logic/game.h"

struct game * game_init(void)
{
    struct game * game = (struct game *)malloc(sizeof(struct game));

    struct point **users_coord = (struct point **)malloc(2 * sizeof(struct point*));
    struct player **users = (struct player **)malloc(2 * sizeof(struct player *));

    for (int i = 0; i < 2; i++)
    {
        users_coord[i] = (struct point *)malloc(sizeof(struct point));

		users_coord[i]->x = i;// ЗДЕСЬ ВСТАВИТЬ НАЧАЛЬНЫЕ КООРДЫ
		users_coord[i]->y = i + 1;// ДЛЯ ИГРОКОВ

        users[i] = user_init(users_coord[i]);
    }

    // инициализация для первого слоя мобов
    struct point **alien_coord = (struct point **)malloc(12 * sizeof(struct point *));
    struct enemy **aliens = (struct enemy **)malloc(12 * sizeof(struct enemy*));

    for (int i = 0; i < 4; i++)
    {
        alien_coord[i] = (struct point *)malloc(sizeof(struct point));

		alien_coord[i]->x = i;// ЗДЕСЬ ВСТАВИТЬ НАЧАЛЬНЫЕ КООРДЫ
		alien_coord[i]->y = i + 1;// ДЛЯ МОБОВ

        aliens[i] = enemy_init(alien_coord[i],WEAK);
    }

    // инициализация для второго слоя мобов
    for (int i = 4; i < 8; i++)
    {
        alien_coord[i] = (struct point *)malloc(sizeof(struct point));

		alien_coord[i]->x = i;// ЗДЕСЬ ВСТАВИТЬ НАЧАЛЬНЫЕ КООРДЫ
		alien_coord[i]->y = i + 1;// ДЛЯ МОБОВ

        aliens[i] = enemy_init(alien_coord[i],MEDIUM);
    }

    // инициализация для третьего слоя мобов
    for (int i = 8; i < 12; i++)
    {
        alien_coord[i] = (struct point *)malloc(sizeof(struct point));

		alien_coord[i]->x = i;// ЗДЕСЬ ВСТАВИТЬ НАЧАЛЬНЫЕ КООРДЫ
		alien_coord[i]->y = i + 1;// ДЛЯ МОБОВ

        aliens[i] = enemy_init(alien_coord[i],HARD);
    }

    game->aliens = aliens;
    game->users = users;
    game->score = 0;
}