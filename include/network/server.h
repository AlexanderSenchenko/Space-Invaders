#ifndef _SERVER_H_
#define _SERVER_H_
#include "../logic/game.h"
#include "../logic/user.h"
#include "../logic/enemy.h"
#include "../logic/bullet.h"

void init_server(int, char **);
void first_reception();
void reception_application();
void create_new_session();

int recv_message(int, struct enemy *, struct player *, struct bullet *, int);

void send_message(int, int, void *);

#endif // _SERVER_H_
