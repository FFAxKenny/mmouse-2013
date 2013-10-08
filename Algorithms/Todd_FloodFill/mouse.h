
#ifndef MOUSE_H
#define MOUSE_H

#include "maze.h"

void ExecuteCmd(Command cmd, int *row, int *col, Direction *dir);
void MapCell(Maze *maze, Maze *map, int row, int col);
int DriveStraight(Maze *maze, int *row, int *col, Direction *dir);

#endif

