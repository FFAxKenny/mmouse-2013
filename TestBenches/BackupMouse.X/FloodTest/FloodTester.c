/*
 *
 *
 * FloodTester.c
 *
 */
#include <stdio.h>
#include "FloodFill.h"
#include "dirdef.h"

Cell mouseMaze[16][16];
StackA nextLevel;
StackA currentLevel;

int main() {
    printf("Init maze\n");
    FloodFill_initMaze();
    printf("Done init maze\n");
    FloodFill_printMaze();
    printf("Modify some values\n");

    //Maze_putWall(mouseMaze, 0, 0, NORTH);
    Maze_putWall(mouseMaze, 0, 0, EAST);
    Maze_putWall(mouseMaze, 1, 0, EAST);
    Maze_putWall(mouseMaze, 2, 1, SOUTH);
    Maze_putWall(mouseMaze, 2, 1, NORTH);
    Maze_putWall(mouseMaze, 2, 2, EAST);
    Maze_putWall(mouseMaze, 2, 2, SOUTH);
    Maze_putWall(mouseMaze, 2, 2, NORTH);
    Maze_putWall(mouseMaze, 3, 0, NORTH);
    Maze_putWall(mouseMaze, 3, 2, NORTH);
    Maze_putWall(mouseMaze, 3, 2, EAST);
    Maze_putWall(mouseMaze, 4, 1, WEST);
    Maze_putWall(mouseMaze, 5, 1, WEST);
    Maze_putWall(mouseMaze, 5, 1, EAST);

    FloodFill_floodMaze();
    FloodFill_printMaze();

    printf("%d\n", Maze_smallestNeighborDir(&mouseMaze[4][1]) );


}
