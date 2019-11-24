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

#define MAX_CLIENT 10
#define MAX_SESSION 3

struct serv_information {
  unsigned int status;
  /* Informing the client: 1 - successful connection and queuing
  2 - Start a game session
  4 - End of session
  8 - Reconnect (after the end of the session)*/
};

struct sockaddr_in addr_server;
struct sockaddr_in addr_client[MAX_CLIENT];
struct sockaddr_in client_session[MAX_SESSION][2];
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

  information_to_player.status = 1;
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

    information_to_player.status = 1;
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

  if ((counter_player >= 0) && (counter_session < MAX_SESSION)) {
    information_to_player.status = 2;

    sendto(file_descrip_server, &information_to_player,
           sizeof(information_to_player), 0,
           (struct sockaddr *)&addr_client[0], addr_in_size);

    sendto(file_descrip_server, &information_to_player,
           sizeof(information_to_player), 0,
           (struct sockaddr *)&addr_client[1],
           addr_in_size);

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
