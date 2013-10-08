
#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdio.h>
#include <time.h>
#include "maze.h"

// --- Generate Binary Maze ---
void GenerateBinaryMaze(Maze *maze);

// --- Display Maze ---
void DisplayMaze(Maze maze, int x, int y);
void DisplayMazeCmd(Maze maze);

#endif

