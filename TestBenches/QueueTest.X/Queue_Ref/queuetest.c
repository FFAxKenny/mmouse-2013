/*
 *          File: queuetest.c
 *        Author: Robert I. Pitts <rip@cs.bu.edu>
 * Last Modified: March 9, 2000
 *         Topic: Queue - Array Implementation - Types
 * ----------------------------------------------------------------
 */

#include <stdio.h>
#include <string.h>
#include "queue.h"

int main(void)
{
  /* Create a new queue of characters. */
  queueADT q1 = QueueCreate();

  /* "Name" is a string to put into the queue. */
  char name[] = "Donny Osmond";
  int length = strlen(name);

  int i;  /* Used to loop through the string. */

  /*
   * Put each character from the string into the
   * queue.  If the queue fills up, then stop and
   * no more characters are entered.
   */
  for (i = 0; i < length; i++) {
    if (QueueIsFull(q1))
      break;
    QueueEnter(q1, name[i]);
  }

  /*
   * Remove each character from the queue and
   * print it out.
   */
  printf("Elements: ");

  while (!QueueIsEmpty(q1)) {
    printf("%c", QueueDelete(q1));
  }

  printf("\n");

  /* We're done with the queue, so free it. */
  QueueDestroy(q1);

  return 0;
}
