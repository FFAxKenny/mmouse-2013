
#include <stdlib.h>
#include <stdio.h>
#include "maze.h"
#include "mouse.h"

int main(void)
{
	Maze maze, map;
	const Position center = {CENTER_CELL, HERE},
	               start  = {START_CELL , START_DIR};
	Position myPos = start;

	InitMaze(maze);
	GenerateBinaryMaze(maze);
	GenerateMaze(maze);
	FloodFill(maze, start, center);
	DrawMaze(maze);

	InitMaze(map);
	DrawMaze(map);

	while(!Middle(myPos))
	{
		MapCell(maze, map, myPos);
		//DrawMaze(map);
		DrawSim(map, myPos);
		//DisplayPos(map, myPos);
		ExecuteCmd(GetCmd(map,myPos,center), &myPos);
		while(getchar() != '\n');

	}

	DrawMaze(maze);
	DrawSim(map, myPos);
	//DisplayPos(map, myPos);

}

