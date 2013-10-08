
#include "list.h"

void InitList(List *list)
{
	list->size = 0;
	list->Head = NULL;
	list->Tail = NULL;
}

int GetSize(List *list)
{ return list->size; }

Entry Remove(List *list, int pos)
{
	ListNode *cur;
	Entry temp;

	if(GetSize(list) <= 0)
		return;
	else if(GetSize(list) == 1)
	{
		cur = list->Head;
		temp = cur->entry;
		list->Head = NULL;
		list->Tail = NULL;
	}
	else if(pos == FRONT || pos < BACK)
	{
		cur = list->Head;
		list->Head = cur->next;
		cur->next->prev = NULL;
	}
	else if(pos == BACK || pos >= GetSize(list) - 1)
	{
		cur = list->Tail;
		cur->prev->next = NULL;
		list->Tail = cur->prev;
	}
	else
	{
		cur = GetPos(list, pos);
		cur->prev->next = cur->next;
		cur->next->prev = cur->prev;
	}
	temp = cur->entry;
	free(cur);
	list->size--;
	return temp;
}

Entry PopFront(List *list)
{ return Remove(list, FRONT); }

Entry PopBack(List *list)
{ return Remove(list, BACK); }

Entry GetList(List *list, int pos)
{ return GetPos(list, pos)->entry; }

Entry BackList(List *list)
{ return GetList(list, BACK); }

Entry FrontList(List *list)
{ return GetList(list, FRONT); }

void Traverse(List *list, void (*visit)(Entry))
{
	ListNode *cur = list->Head;

	while(cur != NULL)
	{
		(*visit)(cur->entry);
		cur = cur->next;
	}
}

void RevTrav(List *list, void (*visit)(Entry))
{
	ListNode *cur = list->Tail;

	while(cur != NULL)
	{
		(*visit)(cur->entry);
		cur = cur->prev;
	}
}

void Insert(List *list, Entry s, int pos)
{
	ListNode *new = malloc(sizeof(ListNode)), *cur;

	if(GetSize(list) <= 0)
	{
		new->next = NULL;
		new->prev = NULL;
		list->Head = new;
		list->Tail = new;
	}
	else if(pos == FRONT || pos < BACK)
	{
		new->next = list->Head;
		new->prev = NULL;
		list->Head = new;
		new->next->prev = new;
	}
	else if(pos == BACK || pos >= GetSize(list))
	{
		new->next = NULL;
		new->prev = list->Tail;
		new->prev->next = new;
		list->Tail = new;
	}
	else
	{
		cur = GetPos(list, pos);
		new->next = cur;
		new->prev = cur->prev;
		new->prev->next = new;
		cur->prev = new;
	}
	new->entry = s;
	list->size++;
}

void PushFront(List *list, Entry s)
{ Insert(list, s, FRONT); }

void PushBack(List *list, Entry s)
{ Insert(list, s, BACK); }

ListNode *GetPos(List *list, int pos)
{
	int i;
	ListNode *temp = list->Head;

	if(pos == BACK || pos >= GetSize(list))
		return list->Tail;

	if(pos == 0 || pos < BACK)
		return list->Head;

	for(i = 0; i < pos; i++)
		temp = temp->next;

	return temp;
}

void DestroyList(List *list)
{
	ListNode *cur = list->Head, *next;

	while(cur != NULL)
	{
		next = cur->next;
		free(cur);
		cur = next;
	}

	list->size = 0;
	list->Head = NULL;
	list->Tail = NULL;
}

