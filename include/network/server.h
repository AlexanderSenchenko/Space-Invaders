#ifndef _SERVER_H_
#define _SERVER_H_
#icnlude "../logic/game.h"
void init_server(int, char **);
void first_reception();
void reception_application();
void create_new_session();
void recv_message(int, struct enemy *, struct user *, struct bullet *)
void send_message(int, int, void *)
#endif // _SERVER_H_
