#ifndef _SERVER_H_
#define _SERVER_H_
#include "../logic/game.h"
#include "../logic/user.h"
#include "../logic/enemy.h"
#include "../logic/bullet.h"
#include "../config.h"

struct message {
  unsigned int status;
  unsigned int id_user;
  unsigned int size_data;
  char data[0];
};

void init_server(int, char **);
void first_reception();
void reception_application();
void create_new_session();

int recv_message(int, struct enemy *, struct player *, struct bullet *);
void send_message(int, int, void *, unsigned int);

#endif // _SERVER_H_
