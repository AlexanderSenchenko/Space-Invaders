#ifndef _CONFIG_H_
#define _CONFIG_H_

// Send To Server
#define STS_END     40
#define STS_MOVE    42
#define STS_BULLET  43

#define MAX_CLIENT  10
#define MAX_SESSION 3

// Send To Client
#define STC_END   50
#define STC_MOVE  52

#define MAX_SIZE_MSG 100

struct message {
  unsigned int status;
  unsigned int id_user;
  unsigned int id_bullet;
  char data[0];
};

#endif
