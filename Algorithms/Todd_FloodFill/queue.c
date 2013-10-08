
#include "queue.h"

void InitQueue(Queue *q)
{ DestroyQueue(q); }

int GetSize(Queue *q)
{ return q->size; }

bool QueueEmpty(Queue *q)
{ return q->size == 0; }

bool QueueFull(Queue *q)
{ return q->tail > MAXQUEUE; }

Entry Front(Queue *q)
{
	if(!QueueEmpty(q))
		return q->entry[q->head];
}

Entry Pop(Queue *q)
{
	int pos;
	if(!QueueEmpty(q))
	{
		q->size--;
		pos = q->head++;
		return q->entry[pos];
	}
}

void Push(Queue *q, Entry s)
{
	if(!QueueFull(q))
	{
		q->size++;
		q->entry[++q->tail] = s;
	}
}

void Traverse(Queue *q, void (*visit)(Entry))
{
	int i = q->head;

	for(; i <= q->tail; i++)
		(*visit)(q->entry[i]);
}

void PrintPos(Entry a)
{
	//printf("%d %d\n", a >> 8, a & 0xFF);
}

void DestroyQueue(Queue *q)
{
	q->size = 0;
	q->head = 0;
	q->tail = -1;
}

