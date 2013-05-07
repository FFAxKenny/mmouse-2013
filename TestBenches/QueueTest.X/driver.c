/* 
 * File:   driver.c
 * Author: kenny
 *
 * Created on April 30, 2013, 5:14 PM
 */

#include <stdio.h>
#include <stdlib.h>

int stack[100];
/*
 * 
 */
int main(int argc, char** argv) {
    init(stack);
    display(stack);
    return (EXIT_SUCCESS);
}

