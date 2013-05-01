/*
 *
 * stackA.h
 *
 */

#include "stack.h"
#include "Cell.h"
#define MAX_SIZE 255

#ifndef STACKA_H
#define STACKA_H

typedef struct stacka{
    int top;
    stackElement elements[MAX_SIZE];
} StackA;

void StackInit(StackA *s);
void StackPush(StackA *s, stackElement e);
stackElement StackPop(StackA *s);
void StackDestroy(StackA *s);
int StackIsEmpty(StackA *s);
int StackIsFull(StackA *s);
void StackCopy(StackA *o, StackA *d);
#endif
