#include "../../include/logic/bullet_list.h"

void Push(struct bullet_list **head, struct bullet *data)
{
	struct bullet_list *tmp = (struct list*)malloc(sizeof(struct bullet_list));
	tmp->data = data;
	tmp->next = NULL;
	if (*head)
	{
		struct bullet_list *p = *head;
	while (p->next != NULL)
		p = p->next;
	p->next = tmp;
	}
	else
	{
		*head = tmp;
	}
}

void Pop(struct bullet_list *head, struct bullet * data)
{
	while (head->next != NULL && head->next->data != data)
		head = head->next;
	if (head->next)
	{
		struct bullet_list *p = head->next;
		head->next = p->next;
		free(p);
	}
}

struct bullet_list * Find(struct bullet_list *head, int id)
{
	while (head && (head->data->id != id))
		head = head->next;
	return head;
}

