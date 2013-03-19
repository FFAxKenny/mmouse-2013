/*
 *  Author: Kenny Luong
 *  Date:   3/11/13
 *  
 *  Maze.c
 *
 *  Loads the maze into the program, and intializes each cell
 *
 *
 */

#include <stdio.h>
#include "rawMazeUtils.h"
#include "MazeUtils.h"
#include "tfdef.h"



int main()
{
    // declare the cells
    Cell maze[16][16];
    char rawMaze[33][65];
    
    readRawMaze(rawMaze);
    printRawMaze(rawMaze);

    checkCell(rawMaze, 0, 0);

    initMaze(maze, rawMaze);
    printMaze(maze);
}








