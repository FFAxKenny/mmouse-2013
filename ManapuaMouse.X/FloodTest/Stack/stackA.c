/*
 *
 * stackA.c
 *
 */
#include "stackA.h"
void StackInit(StackA *s){
    init(&(s->top));
}
void StackPush(StackA *s, stackElement e){
    push( s->elements, &(s->top) , e);
}

stackElement StackPop(StackA *s){
    return pop(s->elements, &(s->top));
}
int StackIsEmpty(StackA *s){
    return empty(&(s->top));
}
int StackIsFull(StackA *s){
    return full(&(s->top), MAX_SIZE);
}
void StackCopy(StackA *o, StackA *d){
    StackA temp;
    StackInit(&temp);

    while(!StackIsEmpty(o)) {
        StackPush(&temp, StackPop(o));
    }
    while(!StackIsEmpty(&temp)) {
        StackPush(d, StackPop(&temp));
    }
}
