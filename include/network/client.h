#ifndef _CLIENT_H_
#define _CLIENT_H_

#include "../logic/game.h"
#include "../logic/user.h"
#include "../logic/enemy.h"
#include "../logic/bullet.h"
#include "../config.h"

struct serv_information {
  unsigned int status;
};

struct message_transmitting{
  unsigned int status;
  unsigned int id_user;
  void *data;
};

struct message {
  unsigned int status;
  unsigned int id_user;
  unsigned int size_data;
  char data[0];
};

/*Теги для status, 1-ая группа технические*/
#define CONNECT   1
#define STRT_GS   2
#define ERR_CONN  3
#define END_GS    9

/*2-ая, группа игровой направлености*/
#define MV_LEFT   30
#define MV_RIGHT  31

void init_client(int, char **);
int reception();
void expectation();

struct message_transmitting wait_start_of_game();

void recv_message(struct game *, struct enemy *, struct player *, struct bullet *);
void send_message(int, int, void *, unsigned int);

#endif // _CLIENT_H_
