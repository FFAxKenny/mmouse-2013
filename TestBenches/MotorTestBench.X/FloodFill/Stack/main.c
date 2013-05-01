/*
 * File:    main.c
 * Author:  zentut.com
 * Purpose: stack test
 */

#include <stdio.h>
#include <stdlib.h>

#include "stack.h"


int main()
{
    const int SIZE = 5; /* stack size */

    int top, elem;
    int stack[SIZE];

    init(&top);
    printf("--Push elements into stack --\n");
    /* push elements into stack */
    while(!full(&top,SIZE))
    {
        printf("Enter a number to push into the stack:");
        scanf("%d",&elem);
        push(stack,&top,elem);
        display(stack,&top);
    }
    printf("Stack is full\n\n");

    printf("--Pop elements into stack --\n");
    while(!empty(&top))
    {
        elem = pop(stack,&top);
        printf("pop %d from the stack\n",elem);

        display(stack,&top);
    }
    printf("Stack is empty\n");
    return 0;
}

