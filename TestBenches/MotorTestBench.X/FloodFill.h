/*
 *
 *
 * FloodFill.h
 *
 */
#include <stdio.h>
#include "Cell.h"
#include "stack.h"
#include "stackA.h"
#include "dirdef.h"
#include "tfdef.h"

#ifndef FLOODFILL_H
#define FLOODFILL_H
extern Cell mouseMaze[16][16];
extern StackA nextLevel;
extern StackA currentLevel;

void FloodFill_pushNeighbors(Cell *c);
void FloodFill_initMaze(void);
int FloodFill_checkWall(Cell *c, int d);
void FloodFill_printMaze(void);
void FloodFill_floodMaze(void);
#endif
