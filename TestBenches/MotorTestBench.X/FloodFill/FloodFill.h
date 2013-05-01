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

typedef struct maze{
    Cell cell[16][16];
} Maze;

Maze internalMaze;
Cell mouseMaze[16][16];

StackA nextLevel;
StackA currentLevel;

void FloodFill_pushNeighbors(Cell *c);
int FloodFill_initMaze(void);
int FloodFill_checkWall(Cell *c, int d);
void FloodFill_printMaze(void);
void FloodFill_floodMaze(void);
