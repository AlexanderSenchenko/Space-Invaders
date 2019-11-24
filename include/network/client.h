#ifndef _CLIENT_H_
#define _CLIENT_H_
#icnlude "../logic/game.h"
void init_client(int, char **);
void reception();
void expectation();
void recv_message(struct game *, struct enemy *, struct user *, struct bullet *)
void send_message(int, int, void *)
#endif // _CLIENT_H_
