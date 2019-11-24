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

struct serv_information {
  unsigned int status;
};

struct sockaddr_in addr_server;
struct serv_information information_to_player;
struct serv_information information_from_player;

socklen_t addr_in_size = sizeof(struct sockaddr_in);
pthread_t receiver_from_server;

int file_descrip_client;

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

void expectation()
{
  int flag = 1;
  recvfrom(file_descrip_client, &information_to_player,
           sizeof(information_to_player), 0,
           (struct sockaddr *)&addr_server, &addr_in_size);

  if (information_to_player.status == 2) {
    //game_session();
  } else {
    while (flag == 1) {
      recvfrom(file_descrip_client, &information_to_player,
               sizeof(information_to_player), 0,
               (struct sockaddr *)&addr_server, &addr_in_size);

      if (information_to_player.status == 2) {
        flag = 0;
      }
    }

    //game_session();
  }
}