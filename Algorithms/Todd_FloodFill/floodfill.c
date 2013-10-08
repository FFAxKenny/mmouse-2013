
#include <stdlib.h>
#include <stdio.h>
#include "maze.h"
#include "mouse.h"
#include "display.h"

int main(void)
{
	int mouseRow = START_ROW, mouseCol = START_COL;
	Direction mouseDir = NORTH;
	Maze maze, map;

	// Generate Random Maze and Store it in 'map'
	GenerateBinaryMaze(&map);
	Maze_FloodfillGP(&map, mouseRow, mouseCol, -1, -1);
	DisplayMaze(map, mouseRow, mouseCol);
	while(1);
	DisplayMaze(map, mouseRow, mouseCol);
	Maze_Floodfill(&map, mouseRow, mouseCol, -1, -1, UNMAPPED);
	//Maze_Floodfill(&map, 14, 10, -1, -1);
	//Maze_FloodfillGP(&map, mouseRow, mouseCol, -1, -1);
	DisplayMaze(map, mouseRow, mouseCol);

	// Initialize Maze for the Mouse
	Maze_Init(&maze);
	DisplayMaze(maze, mouseRow, mouseCol);
	DisplayMaze(map, mouseRow, mouseCol);
	Maze_Init(&maze);

	while(!CenterPos(mouseRow,mouseCol))
	{
		MapCell(&maze,&map,mouseRow,mouseCol);
		//DisplayMaze(maze, mouseRow, mouseCol);
		//while(getchar() != '\n');
		ExecuteCmd(Maze_GetCmd(&maze,mouseRow,mouseCol,mouseDir,-1,-1),&mouseRow,&mouseCol,&mouseDir);
	}

	while(!StartPos(mouseRow,mouseCol))
	{
		MapCell(&maze,&map,mouseRow,mouseCol);
		//DisplayMaze(maze, mouseRow, mouseCol);
		//while(getchar() != '\n');
		ExecuteCmd(Maze_GetCmd(&maze,mouseRow,mouseCol,mouseDir,15,0),&mouseRow,&mouseCol,&mouseDir);
	}

	//Maze_Floodfill(&maze, mouseRow, mouseCol, -1, -1, MAPPED);
	//DisplayMaze(maze, mouseRow, mouseCol);
	mouseDir = NORTH;
	Maze_SetCmd(&maze,mouseRow,mouseCol,mouseDir);
	//DisplayMaze(maze, mouseRow, mouseCol);
	DisplayMazeCmd(maze);

}

