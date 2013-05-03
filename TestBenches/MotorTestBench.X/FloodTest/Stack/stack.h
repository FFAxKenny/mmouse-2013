/*
 * File:   stack.h
 * Author: zentut.com
 * Purpose: stack header file
 */
#include "Cell.h"

#ifndef STACK_H_INCLUDED
#define STACK_H_INCLUDED
typedef Cell* stackElement;
void push(stackElement *s, int* top, stackElement element);
stackElement pop(stackElement *s,int *top);
int full(int *top,const int size);
int empty(int *top);
void init(int *top);
void display(stackElement *s,int *top);

#endif // STACK_H_INCLUDED
