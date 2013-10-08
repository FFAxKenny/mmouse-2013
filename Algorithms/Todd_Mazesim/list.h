
#ifndef LIST_H
#define LIST_H

#include "common.h"
//#include <stdlib.h>
//#include <stdio.h>

#define FRONT 0
#define BACK -1

/*-----------------------*
 *    ENTRY DATA TYPE    *
 *-----------------------*/
typedef Position Entry;

/*----------------------*
 *    LISTNODE STRUCT   *
 *----------------------*/
typedef struct listnode
{
	Entry entry;
	struct listnode *next;
	struct listnode *prev;
} ListNode;

/*------------------*
 *    LIST STRUCT   *
 *------------------*/
typedef struct
{
	int size;
	ListNode *Head;
	ListNode *Tail;
} List;

/*--------------------------*
 *    INITIALIZE FUNCTION   *
 *--------------------------*/
void InitList(List *list);

/*-------------------------*
 *    ACCESSOR FUNCTIONS   *
 *-------------------------*/
int GetSize(List *list);
Entry Remove(List *list, int pos);
Entry PopFront(List *list);
Entry PopBack(List *list);
Entry GetList(List *list, int pos);
Entry BackList(List *list);
Entry FrontList(List *list);
void Traverse(List *list, void (*visit)(Entry));
void RevTrav(List *list, void (*visit)(Entry));

/*------------------------*
 *    MUTATOR FUNCTIONS   *
 *------------------------*/
void Insert(List *list, Entry s, int pos);
void PushFront(List *list, Entry s);
void PushBack(List *list, Entry s);

/*----------------------*
 *    HELPER FUNCTION   *
 *----------------------*/
ListNode *GetPos(List *list, int pos);

/*--------------------------*
 *    DESTRUCTOR FUNCTION   *
 *--------------------------*/
void DestroyList(List *list);

#endif

