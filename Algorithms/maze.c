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
    Cell real_maze[16][16];
    Cell mouse_maze[16][16];

    char rawMaze[33][65];
    
    readRawMaze(rawMaze);       // Reads the raw maze into char array
    printRawMaze(rawMaze);      // Prints it out so we cna double check..

    initMaze(mouse_maze);
    initMazeFromRaw(real_maze, rawMaze);
    printMaze(mouse_maze);      // Prints the maze's values

    checkCell(rawMaze, 0, 0);   

    // Run Main maze program

}
