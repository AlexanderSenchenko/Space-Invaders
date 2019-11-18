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

#define MAX_CLIENT 10
#define MAX_LENGHT_NAME 20
#define MAX_SESSION 4


struct serv_information
{
  unsigned int status;
  /* Informing the client: 1 - successful connection and queuing
  2 - Start a game session
  4 - End of session
  8 - Reconnect (after the end of the session)*/
};

struct sockaddr_in adrr_server;
struct sockaddr_in adrr_client[MAX_CLIENT];
struct sockaddr_in client_session[MAX_SESSION][2];
struct serv_information information_to_player;
struct serv_information information_from_player;
socklen_t addr_in_size = sizeof(struct sockaddr_in);

char address_server[15];

int port_server;
int counter_player = 0;  // Counter responsible for counting connected players
int counter_session = 0;
int file_descrip_server;

void init_preference();
void first_reception();
void reception_application();

int main()
{
  init_preference();
  first_reception();
  /*Note: We create a stream to process all of the following requests*/
}
void init_preference()  // Set up the connection
{
  char answer;
  printf("Do you want to put you setting[Y/N]\n");
  answer = getchar();
  if((answer == 'Y') || ((answer == 'y')))
  {
    printf("Your setting:\n");
    printf("IP-address: ");
    fgets(address_server, 16, stdin);
    printf("Port: ");
    scanf("%d\n", &port_server);
  }
  else
  {
    printf("Default setting:\n");
    port_server = 8974;
    strcpy(address_server, "127.0.0.1");
  }
}
void first_reception()  // Waiting for the first to connect
{
  adrr_server.sin_family = AF_INET;
  adrr_server.sin_port = htons(port_server);
  inet_aton((const char *)&address_server, &adrr_server.sin_addr);
  file_descrip_server = socket(AF_INET, addr_in_size, 0);
  bind(file_descrip_server, (struct sockaddr *)&adrr_server, sizeof(struct sockaddr_in));
  recvfrom(file_descrip_server, &information_from_player, sizeof(information_from_player), 0, (struct sockaddr *)&adrr_client[counter_player], &addr_in_size);
  information_to_player.status = 1;
  sendto(file_descrip_server, &information_to_player, sizeof(information_to_player), 0, (struct sockaddr *)&adrr_client[0], addr_in_size);
  counter_player++;
}
void reception_application()  // For subsequent connecting
{
  if(counter_player < MAX_CLIENT)
  {
    recvfrom(file_descrip_server, &information_from_player, sizeof(information_from_player), 0, (struct sockaddr *)&adrr_client[counter_player], &addr_in_size);
    information_to_player.status = 1;
    sendto(file_descrip_server, &information_to_player, sizeof(information_to_player), 0, (struct sockaddr *)&adrr_client[0], addr_in_size);
    counter_player++;
  }
}
void create_new_session() // Create a new game session
{
  int i;
  int number_session;

  if((counter_player > 2) && (counter_session < MAX_SESSION))
  {
    information_to_player.status = 2;
    sendto(file_descrip_server, &information_to_player, sizeof(information_to_player), 0, (struct sockaddr *)&adrr_client[0], addr_in_size);
    sendto(file_descrip_server, &information_to_player, sizeof(information_to_player), 0, (struct sockaddr *)&adrr_client[1], addr_in_size);
    /* Create a process or thread as a game session. We give them a fork and shift the structure by 2 */
    for(i = 0; i < counter_player-2; i++)
    {
      adrr_client[i] = adrr_client[i+1];
      adrr_client[i+1] = adrr_client[i+2];
    }
    counter_player = counter_player - 2;
    counter_session++;
  }

}
