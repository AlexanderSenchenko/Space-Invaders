#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <arpa/inet.h>
#include "../../include/network/client.h"
#include "../../include/logic/game.h"
#include "../../include/logic/game.h"
#include "../../include/logic/user.h"
#include "../../include/logic/enemy.h"
#include "../../include/logic/bullet.h"



/*Теги для status, 1-ая группа технические*/
#define CONNECT 1
#define STRT_GS 2
#define ERR_CONN 3
#define END_GS 9

/*2-ая, группа игровой направлености*/
#define MV_LEFT 30
#define MV_RIGHT 31


struct sockaddr_in addr_server;
struct serv_information information_to_player;
struct serv_information information_from_player;

struct message_transmitting message;

socklen_t addr_in_size = sizeof(struct sockaddr_in);
pthread_t receiver_from_server;

int file_descrip_client;
int id;

void init_client(int argc, char **argv)
{
  addr_server.sin_family = AF_INET;

  if (argc == 3) {
    inet_aton(argv[1], &addr_server.sin_addr);
    addr_server.sin_port = htons(atoi(argv[2]));
  } else if (argc == 2) {
    inet_aton(argv[1], &addr_server.sin_addr);
    addr_server.sin_port = htons(8974);
  } else if (argc == 1) {
    inet_aton("127.0.0.1", &addr_server.sin_addr);
    addr_server.sin_port = htons(8974);
  }
}

int reception()
{
  file_descrip_client = socket(AF_INET, SOCK_DGRAM, 0);
  information_from_player.status = 1;

  sendto(file_descrip_client, &information_from_player,
         sizeof(information_from_player), 0,
         (struct sockaddr *)&addr_server, addr_in_size);

  recvfrom(file_descrip_client, &information_to_player,
           sizeof(information_to_player), 0,
           (struct sockaddr *)&addr_server, &addr_in_size);

  return information_to_player.status;
}

void *receiver() /*Заготовка*/
{
  while (1) {
    recvfrom(file_descrip_client, &information_to_player,
             sizeof(information_to_player), 0,
             (struct sockaddr *)&addr_server, &addr_in_size);
    /*Отправляет данные в функцию game_session*/
  }
}

/*void game_session()
{
  void *status;
  i_error = pthread_create(&receiver_from_server, NULL, receiver, NULL);
  error_output(ERROR_PTHREAD);
  /*Игровая логика*//*
  i_error = pthread_cancel(receiver_from_server);
  error_output(ERROR_PTHREAD);
  i_error = pthread_join(receiver_from_server, &status);
  error_output(ERROR_PTHREAD);
}*/

struct message_transmitting wait_start_of_game()
{
  recvfrom(file_descrip_client, &message,
           sizeof(struct message_transmitting), 0,
           (struct sockaddr *) &addr_server, &addr_in_size);

  return message;
}

void expectation()
{
  struct game game_ses;
  unsigned int id;
  int flag = 1;

  recv_message(&game_ses, NULL, NULL, NULL);
  id = message.id_user;

  recvfrom(file_descrip_client, &information_to_player,
           sizeof(information_to_player), 0,
           (struct sockaddr *)&addr_server, &addr_in_size);

  if (information_to_player.status == STRT_GS) {

    //game_session();
  } else {
    while (flag == 1) {
      recvfrom(file_descrip_client, &information_to_player,
               sizeof(information_to_player), 0,
               (struct sockaddr *)&addr_server, &addr_in_size);

      if (information_to_player.status == STRT_GS) {
        flag = 0;
      }
    }

    //game_session();
  }
}

void recv_message(struct game *game_mess, struct enemy *enemy_mess,
                  struct player *user_mess, struct bullet *bullet__mess)
{
  struct message msg;
  char message[MAX_SIZE_MSG];

  recvfrom(file_descrip_client, &message,
           sizeof(message), 0,
           (struct sockaddr *) &addr_server, &addr_in_size);

  memcpy(&msg, message, sizeof(struct message));

  switch (msg.status) {
  case STRT_GS:

    break;

  case MV_LEFT:

    break;

  case MV_RIGHT:


    break;

    /*И другие*/
  }
}

int recv_message_dontwait(struct player *user)
{
  struct message msg;
  char message[MAX_SIZE_MSG];

  recvfrom(file_descrip_client, &message,
           sizeof(message), MSG_DONTWAIT,
           (struct sockaddr *) &addr_server, &addr_in_size);

  memcpy(&msg, message, sizeof(struct message));

  switch (msg.status) {
  case STC_END:
    return STC_END;
    break;

  case STC_MOVE:
    user->id = msg.id_user;
    memcpy(user->coord, message + sizeof(struct message),
           sizeof(struct point));
    return STC_MOVE;
    break;

  default:
    break;
  }

  return 0;
}

void send_message(int status, int id_user, void *data, unsigned int size_data)
{
  unsigned int size_msg = sizeof(struct message) + size_data;

  struct message *msg = calloc(1, size_msg);
  msg->status = status;
  msg->id_user = id_user;
  // msg->size_data = size_data;

  if (status == STS_BULLET) {
    msg->id_bullet = ((struct bullet*) data)->id;
    memcpy(msg->data, ((struct bullet*) data)->coord, size_data);
  } else if (data != NULL) {
    memcpy(msg->data, data, size_data);
  }

  sendto(file_descrip_client, msg, size_msg, 0,
         (struct sockaddr *) &addr_server, addr_in_size);

  if (msg != NULL)
    free(msg);
}