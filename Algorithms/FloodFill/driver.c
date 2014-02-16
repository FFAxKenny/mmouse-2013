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
#include "RawMaze.h"
#include "Maze.h"
#include "tfdef.h"

int main()
{
    // declare the cells
    Cell maze[16][16];
    Cell real_maze[16][16];
    Cell mouse_maze[16][16];

    char rawMaze[33][65];

    RawMaze_readFile(rawMaze);       // Reads the raw maze into char array
    
    /* RawMaze_read(rawMaze);       // Reads the raw maze into char array */
    RawMaze_print(rawMaze);      // Prints it out so we cna double check..

    Maze_initFromRaw(real_maze, rawMaze);    // init the wall values
    Maze_print(real_maze);                   // print the initial flood value

    Maze_flood(real_maze);                   // flood the maze with one layer
    Maze_print(real_maze);                   // print the initial flood value
}
