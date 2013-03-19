#include <stdio.h>
#include "rawMazeUtils.h"
#include "tfdef.h"

#define maze_height 16
#define maze_width  16
#define MAZE_HEIGHT 16
#define MAZE_WIDTH  16

typedef struct{
     int floodValue;
     int x;
     int y;
     int north;
     int east;
     int south;
     int west;
}Cell;

void printMaze(Cell maze[maze_height][maze_width]);
void initMaze(Cell maze[maze_height][maze_width]);

