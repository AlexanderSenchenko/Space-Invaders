#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <arpa/inet.h>
#include "../../include/network/server.h"
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
pthread_t new_flow; /*Временное*/
pthread_mutex_t latch = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mtx_one = PTHREAD_MUTEX_INITIALIZER;

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
             (struct sockaddr *) &addr_client[counter_player],
             &addr_in_size);

    information_to_player.status = CONNECT;
    sendto(file_descrip_server, &information_to_player,
           sizeof(information_to_player), 0,
           (struct sockaddr *) &addr_client[counter_player], addr_in_size);

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
  struct sockaddr_in addr_client_session[2];

  if ((counter_player > 1) && (counter_session < MAX_SESSION)) {

    send_message(STRT_GS, 0, NULL, 0);
    send_message(STRT_GS, 1, NULL, 0);

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

  // временный цикл, для испровления отпраыки сообщени
  struct point *plr_coord = calloc(1, sizeof(struct point));
  struct player *plr = user_init(plr_coord);

  struct point *bull_coord = calloc(1, sizeof(struct point));
  struct bullet *bull = calloc(1, sizeof(struct bullet));
  bull->coord = bull_coord;

  struct game *game_ses = game_init();
  game_ses->user->id = 0;
  game_ses->second_user->id = 1;

  pthread_mutex_init(&mtx_one, NULL);
  pthread_create(&new_flow, NULL, new_function, NULL);

  while (1) {
    int exit_stauts = recv_message(0, NULL, plr, bull);

    if (exit_stauts == STS_END) {
      break;
    } else if (exit_stauts == STS_MOVE) {
      update_player_coord(game_ses, plr->id, plr->coord);
      send_message(STC_MOVE, plr->id ^ 1, plr->coord, sizeof(struct point));
    } else if (exit_stauts == STS_BULLET) {
      add_bullet(game_ses, plr->id, bull);
      printf("Idu: %d, Idb: %d, x: %d, y: %d\n", plr->id, bull->id,
             bull->coord->x, bull->coord->y);
    }
  }

  user_dest(plr);
  bullet_dest(bull);
  free_game(game_ses);
  function_closed_server();
}

void update_player_coord(struct game *game_ses, int id, struct point *coord)
{
  if (id == 0) {
    memcpy(game_ses->user->coord, coord, sizeof(struct point));
  } else if (id == 1) {
    memcpy(game_ses->second_user->coord, coord, sizeof(struct point));
  } else {
    // Error
  }
}

void add_bullet(struct game *game_ses, int id, struct bullet *tmp_bull)
{
  struct bullet *bull = calloc(1, sizeof(struct bullet));
  memcpy(bull, tmp_bull, sizeof(struct bullet));

  if (id == 0) {
    Push(&game_ses->user->list, bull);
  } else if (id == 1) {
    Push(&game_ses->second_user->list, bull);
  } else {
    // Error
    bullet_dest(bull);
  }
}

// TODO: переделать send_message для отправки bullet
void send_message(int status, int id_user, void *data, unsigned int size_data)
{
  unsigned int size_msg = sizeof(struct message) + size_data;

  struct message *msg = calloc(1, size_msg);
  msg->status = status;
  msg->id_user = id_user;
  // msg->size_data = size_data;

  if (data != NULL)
    memcpy(msg->data, data, size_data);

  sendto(file_descrip_server, msg, size_msg, 0,
         (struct sockaddr *) &addr_client[id_user], addr_in_size);

  if (msg != NULL)
    free(msg);
}

int recv_message(int id_user, struct enemy *enemy_mess,
                 struct player *user_mess, struct bullet *bullet__mess)
{
  struct message msg;
  char message[MAX_SIZE_MSG];
  struct sockaddr_in addr_client;
  int ret;

  ret = recvfrom(file_descrip_server, message,
                 MAX_SIZE_MSG, 0,
                 (struct sockaddr *) &addr_client, &addr_in_size);

  memcpy(&msg, message, sizeof(struct message));

  switch (msg.status) {
  case STS_MOVE:
    user_mess->id = msg.id_user;
    memcpy(user_mess->coord, message + sizeof(struct message),
           sizeof(struct point));
    return STS_MOVE;

  case STS_BULLET:
    user_mess->id = msg.id_user;
    bullet__mess->id = msg.id_bullet;
    memcpy(bullet__mess->coord, message + sizeof(struct message),
           sizeof(struct bullet));
    // printf("Act: STS_BULLET, Idu: %d\n", msg.id_user);
    // printf("Idb: %d, x: %d, y: %d\n", bullet__mess->id,
    //        bullet__mess->coord->x, bullet__mess->coord->y);
    return STS_BULLET;

  case STS_END:
    return STS_END;
    /*И другие*/
  }

  return 0;
}

void function_closed_server()
{
  close(file_descrip_server);
  pthread_mutex_destroy(&latch);
  pthread_mutex_destroy(&mtx_one);
}

void *new_function() //придумать имя
{
  struct timeval time;

  int flag;
  int data; //Заменить на нужное
  int i;

  while (1) {
    time.tv_sec = 1L; //настроить секунды
    time.tv_usec = 1L; // Настроить микросекунды
    select(0, NULL, NULL, NULL, &time); //Вроде бы может зайти за таймер
    i++;
  }
}