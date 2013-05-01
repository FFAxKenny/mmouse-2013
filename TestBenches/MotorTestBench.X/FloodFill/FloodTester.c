/*
 *
 *
 * FloodTester.c
 *
 */
#include <stdio.h>
#include "FloodFill.h"
#include "dirdef.h"

int main() {
    printf("Init maze\n");
    FloodFill_initMaze();
    printf("Done init maze\n");
    FloodFill_printMaze();
    printf("Modify some values\n");

    Maze_putWall(&mouseMaze, 0, 0, NORTH);
    Maze_putWall(&mouseMaze, 0, 1, NORTH);
    Maze_putWall(&mouseMaze, 0, 2, NORTH);
    Maze_putWall(&mouseMaze, 0, 3, NORTH);
    Maze_putWall(&mouseMaze, 0, 4, NORTH);
    Maze_putWall(&mouseMaze, 0, 5, NORTH);
    Maze_putWall(&mouseMaze, 0, 6, NORTH);
    Maze_putWall(&mouseMaze, 0, 7, NORTH);
    Maze_putWall(&mouseMaze, 0, 8, NORTH);
    Maze_putWall(&mouseMaze, 0, 9, NORTH);
    Maze_putWall(&mouseMaze, 0, 10, NORTH);
    Maze_putWall(&mouseMaze, 0, 11, NORTH);
    Maze_putWall(&mouseMaze, 0, 12, NORTH);
    Maze_putWall(&mouseMaze, 0, 13, NORTH);

    FloodFill_floodMaze();
    FloodFill_printMaze();
}
