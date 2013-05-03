/*
 *
 * mainA.c
 *
 */

#include <stdio.h>
#include "stackA.h"

int main()
{
    StackA stack;

    printf("Init stack\n");
    StackInit(&stack);

    printf("Push 1\n");
    StackPush(&stack, 1);
    printf("Push 2\n");
    StackPush(&stack, 2);
    printf("Push 3\n");
    StackPush(&stack, 3);

    printf("Time to pop!!\n");

    while(!StackIsEmpty(&stack) ) {
        printf("%d\n", StackPop(&stack));
    }

}

