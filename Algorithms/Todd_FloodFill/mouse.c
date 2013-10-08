
#include "mouse.h"

void ExecuteCmd(Command cmd, int *row, int *col, Direction *dir)
{

	switch(cmd)
	{
		case FORWARD:                        break;
		case STOP:    *dir =  BackDir(*dir); break;
		case LEFT90:  *dir =  LeftDir(*dir); break;
		case RIGHT90: *dir = RightDir(*dir); break;
	}
	Move(*row, *col, *dir);
}

void MapCell(Maze *maze, Maze *map, int row, int col)
{
	Maze_MapWall(maze,row,col,NORTH,Maze_GetWall(map,row,col,NORTH),UNMAPPED);
	Maze_MapWall(maze,row,col, EAST,Maze_GetWall(map,row,col, EAST),UNMAPPED);
	Maze_MapWall(maze,row,col,SOUTH,Maze_GetWall(map,row,col,SOUTH),UNMAPPED);
	Maze_MapWall(maze,row,col, WEST,Maze_GetWall(map,row,col, WEST),UNMAPPED);
}

int DriveStraight(Maze *maze, int *row, int *col, Direction *dir)
{
	int i;
	for(i = 0; maze->cell[*row][*col].cmd == FORWARD; i++)
		Move(*row, *col, *dir);
	return i = 1;
}

