#include "../../include/logic/game.h"

game_t * game_init(void)
{
    game_t * game = (game_t *)malloc(sizeof(game_t));

    point_t **users_coord = (point_t **)malloc(2 * sizeof(point_t*));
    player_t **users = (player_t **)malloc(2 * sizeof(player_t *));

    for (int i = 0; i < 2; i++)
    {
        users_coord[i] = (point_t *)malloc(sizeof(point_t));

		users_coord[i]->x = i;// ЗДЕСЬ ВСТАВИТЬ НАЧАЛЬНЫЕ КООРДЫ
		users_coord[i]->y = i + 1;// ДЛЯ ИГРОКОВ

        users[i] = user_init(users_coord[i]);
    }

    // инициализация для первого слоя мобов
    point_t **alien_coord = (point_t **)malloc(12 * sizeof(point_t *));
    enemy_t **aliens = (enemy_t **)malloc(12 * sizeof(enemy_t*));

    for (int i = 0; i < 4; i++)
    {
        alien_coord[i] = (point_t *)malloc(sizeof(point_t));

		alien_coord[i]->x = i;// ЗДЕСЬ ВСТАВИТЬ НАЧАЛЬНЫЕ КООРДЫ
		alien_coord[i]->y = i + 1;// ДЛЯ МОБОВ

        aliens[i] = enemy_init(alien_coord[i],WEAK);
    }

    // инициализация для второго слоя мобов
    for (int i = 4; i < 8; i++)
    {
        alien_coord[i] = (point_t *)malloc(sizeof(point_t));

		alien_coord[i]->x = i;// ЗДЕСЬ ВСТАВИТЬ НАЧАЛЬНЫЕ КООРДЫ
		alien_coord[i]->y = i + 1;// ДЛЯ МОБОВ

        aliens[i] = enemy_init(alien_coord[i],MEDIUM);
    }

    // инициализация для третьего слоя мобов
    for (int i = 8; i < 12; i++)
    {
        alien_coord[i] = (point_t *)malloc(sizeof(point_t));

		alien_coord[i]->x = i;// ЗДЕСЬ ВСТАВИТЬ НАЧАЛЬНЫЕ КООРДЫ
		alien_coord[i]->y = i + 1;// ДЛЯ МОБОВ

        aliens[i] = enemy_init(alien_coord[i],HARD);
    }

    game->aliens = aliens;
    game->users = users;
    game->score = 0;
}