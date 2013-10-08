
#ifndef LIST_H
#define LIST_H

//#include <stdio.h>
#include <stdbool.h>

#define MAXQUEUE 256
//#define MAXQUEUE 10000

#define FRONT 0
#define BACK -1

/*-----------------------*
 *    ENTRY DATA TYPE    *
 *-----------------------*/
//typedef Position Entry;
typedef int Entry;

/*------------------*
 *    LIST STRUCT   *
 *------------------*/
typedef struct
{
	int size;
	int head;
	int tail;
	Entry entry[MAXQUEUE];
}
Queue;

/*--------------------------*
 *    INITIALIZE FUNCTION   *
 *--------------------------*/
void InitQueue(Queue *q);

/*----------------------------*
 *    BASIC QUEUE FUNCTIONS   *
 *----------------------------*/
int GetSize(Queue *q);
bool QueueEmpty(Queue *q);
bool QueueFull(Queue *q);
Entry Front(Queue *q);
Entry Pop(Queue *q);
void Push(Queue *q, Entry s);
void Traverse(Queue *q, void (*visit)(Entry));

/*-------------------------------*
 *    SPECIFIC QUEUE FUNCTIONS   *
 *-------------------------------*/
void PrintPos(Entry a);

/*--------------------------*
 *    DESTRUCTOR FUNCTION   *
 *--------------------------*/
void DestroyQueue(Queue *q);

#endif

