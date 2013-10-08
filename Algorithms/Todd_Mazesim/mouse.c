
#include "mouse.h"

void ExecuteCmd(Command cmd, Position *myPos)
{

	switch(cmd)
	{
		case FORWARD: 
			break;
		case STOP: 
			myPos->dir = BackDir(myPos->dir);
			break;
		case FASTER: 
			break;
		case SLOWER: 
			break;
		case LEFT45: 
			break;
		case LEFT90: 
			myPos->dir = LeftDir(myPos->dir);
			break;
		case RIGHT45: 
			break;
		case RIGHT90: 
			myPos->dir = RightDir(myPos->dir);
			break;
	}
	Move(myPos, myPos->dir);

}

void MapCell(Maze maze, Maze map, Position myPos)
{
	myPos.dir = HERE;

	SetWall(map, myPos, GetWall(maze,myPos,NORTH), NORTH);
	SetWall(map, myPos, GetWall(maze,myPos,EAST), EAST);
	SetWall(map, myPos, GetWall(maze,myPos,SOUTH), SOUTH);
	SetWall(map, myPos, GetWall(maze,myPos,WEST), WEST);

}





