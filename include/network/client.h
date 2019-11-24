#ifndef _CLIENT_H_
#define _CLIENT_H_
#include "../logic/game.h"
#include "../logic/user.h"
#include "../logic/enemy.h"
#include "../logic/bullet.h"
void init_client(int, char **);
void reception();
void expectation();
void recv_message(struct game *, struct enemy *, struct player *, struct bullet *);
void send_message(int, int, void *);
#endif // _CLIENT_H_
