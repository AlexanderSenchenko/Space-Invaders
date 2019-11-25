#include "bullet.h"

struct bullet_list
{
    struct bullet * data;
    struct bullet_list * next;
};

void Push(struct bullet_list **head, struct bullet *data);

void Pop(struct bullet_list *head, unsigned int id);

struct bullet_list* Find(struct bullet_list *head, unsigned int id);
