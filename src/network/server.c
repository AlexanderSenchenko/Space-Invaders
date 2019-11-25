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
#include "../../include/network/server.h"
#include "../../include/logic/game.h"
#include "../../include/logic/user.h"
#include "../../include/logic/enemy.h"
#include "../../include/logic/bullet.h"

#define MAX_CLIENT 10
#define MAX_SESSION 3
/*Теги для status, 1-ая группа технические*/
#define CONNECT 1
#define STRT_GS 2
#define ERR_CONN 3
#define END_GS 9
/*2-ая, группа игровой направлености*/
#define MV_LEFT 30
#define MV_RIGHT 31


struct serv_information {
  unsigned int status;
};

struct message_transmitting {
  unsigned int status;
  unsigned int id_user;
  void *data;
};


struct message_transmitting message;
struct sockaddr_in addr_server;
struct sockaddr_in addr_client[MAX_CLIENT];
struct serv_information information_to_player;
struct serv_information information_from_player;
socklen_t addr_in_size = sizeof(struct sockaddr_in);
pthread_t receiver_client;
pthread_mutex_t latch = PTHREAD_MUTEX_INITIALIZER;

int counter_player = 0;  // Counter responsible for counting connected players
int counter_session = 0;
int file_descrip_server;


void init_server(int argc, char **argv)
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

void first_reception()  // Первый контакт, запуск дальнейшей работы сервера
{
  file_descrip_server = socket(AF_INET, SOCK_DGRAM, 0);
  bind(file_descrip_server, (struct sockaddr *)&addr_server,
       sizeof(struct sockaddr_in));

  recvfrom(file_descrip_server, &information_from_player,
           sizeof(information_from_player), 0,
           (struct sockaddr *)&addr_client[counter_player], &addr_in_size);

  information_to_player.status = CONNECT;
  sendto(file_descrip_server, &information_to_player,
         sizeof(information_to_player), 0,
         (struct sockaddr *)&addr_client[0], addr_in_size);

  counter_player++;
  pthread_mutex_init(&latch, NULL);
}

// Должен быть в отдельном потоке для приёма заявок новых клиентов
void reception_application()
{
  if (counter_player < MAX_CLIENT) {
    recvfrom(file_descrip_server, &information_from_player,
             sizeof(information_from_player), 0,
             (struct sockaddr *)&addr_client[counter_player],
             &addr_in_size);

    information_to_player.status = CONNECT;
    sendto(file_descrip_server, &information_to_player,
           sizeof(information_to_player), 0,
           (struct sockaddr *)&addr_client[counter_player], addr_in_size);

    pthread_mutex_lock(&latch);
    counter_player++;
    pthread_mutex_unlock(&latch);

  }
}

//Оповещение обоих игроков, о начале игры, создание сессии
void create_new_session()
{
  int i;
  int number_session;
  int id_user;
  struct game game_ses;
  struct sockaddr_in addr_client_session[2];

  if ((counter_player > 1) && (counter_session < MAX_SESSION)) {
    send_message(STRT_GS, 0, (void *)&game_ses);
    send_message(STRT_GS, 1, (void *)&game_ses);

    addr_client_session[0] = addr_client[0];
    addr_client_session[1] = addr_client[1];

    /*
     * Create a process or thread as a game session.
     * We give them a fork and shift the structure by 2
     */

    pthread_mutex_lock(&latch);

    for (i = 0; i < counter_player - 2; i++) {
      addr_client[i] = addr_client[i + 1];
      addr_client[i + 1] = addr_client[i + 2];
    }

    counter_player = counter_player - 2;
    counter_session++;
    pthread_mutex_unlock(&latch);
  }
}

void send_message(int status, int id_user, void *data)
{
  message.status = status;
  message.id_user = id_user;
  message.data = &data;

  sendto(file_descrip_server, &message,
         sizeof(message), 0,
         (struct sockaddr *)&addr_client[id_user], addr_in_size);/*надо посмотреть id_user*/
}

int recv_message(int id_user, struct enemy *enemy_mess, struct player *user_mess, struct bullet *bullet_mess, int flag)
{
  recvfrom(file_descrip_server, &message,
           sizeof(message), flag,
           (struct sockaddr *)&addr_client[id_user], &addr_in_size);

  switch (message.status) {
  case MV_LEFT:

    return MV_LEFT;
    break;
  case MV_RIGHT:

    return MV_RIGHT;

    break;

  /*И другие*/
  }
}
void receiver_session()
{
  struct enemy enemy_mess;
  struct player user_mess;
  struct bullet bullet_mess;
  int flag = STRT_GS;
  int const_return = -2;
  while(END_GS != flag)
  {
    const_return = -2;
    const_return = recv_message(0, &enemy_mess, &user_mess, &bullet_mess, MSG_DONTWAIT);
    if(-2 != const_return);
    {
      printf("User ID: 0 did the action. %d\n", const_return);
    }
    const_return = -1;
    const_return = recv_message(1, &enemy_mess, &user_mess, &bullet_mess, MSG_DONTWAIT);
    if(-2 != const_return);
    {
      printf("User ID: 1 did the action. %d\n", const_return);
    }
  }
}/*На будущее надо либо нумеровать сессии для логов, либо надо будет для каждой сессии будет создавать свой файл с логами*/
