#include "../include/network/server.h"

int main(int argc, char **argv)
{
  init_server(argc, argv);
  first_reception();
  /*Note: We create a stream to process all of the following requests*/
}