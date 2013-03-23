#include <stdio.h>
#include "RawMaze.h"
#include "tfdef.h"

#define maze_height 16
#define maze_width  16
#define MAZE_HEIGHT 16
#define MAZE_WIDTH  16

#define NORTH 0
#define SOUTH 1
#define EAST  2
#define WEST  3

typedef struct{
     int floodValue;
     int x;
     int y;
     int north;
     int east;
     int south;
     int west;
}Cell;

void Maze_print(Cell maze[maze_height][maze_width]);
void Maze_init(Cell maze[maze_height][maze_width]);
void Maze_initFromRaw(Cell maze[maze_height][maze_width], char rawMaze[RAWH][RAWW]);

