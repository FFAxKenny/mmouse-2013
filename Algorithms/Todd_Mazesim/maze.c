
#include "maze.h"

void InitMaze(Maze maze)
{
	int i, j;
	Position wallInit;
	Cell cellInit;
	     cellInit.flood = 0;
	     cellInit.north = NOWALL;
	     cellInit.west  = NOWALL;
	     cellInit.north_mapped = UNMAPPED;
	     cellInit.west_mapped  = UNMAPPED;
	     cellInit.visited = UNMAPPED;
	     cellInit.command = 0;

	for(i = 0; i < MAX_ROW; i++)
		for(j = 0; j < MAX_COL; j++)
			maze[i][j] = cellInit;

	wallInit.col = 0;
	wallInit.dir = HERE;
	for(i = 0; i < MAX_ROW; i++)
	{
		wallInit.row = i;
		SetWall(maze, wallInit, WALL, WEST);
	}

	wallInit.row = 0;
	wallInit.dir = HERE;
	for(j = 0; j < MAX_COL; j++)
	{
		wallInit.col = j;
		SetWall(maze, wallInit, WALL, NORTH);
	}

	for(i = 0; i < MAX_ROW/2; i++)
		for(j = 0; j < MAX_ROW/2; j++)
		{
			maze[          i][          j].flood = 14-(i+j);
			maze[MAX_ROW-1-i][          j].flood = 14-(i+j);
			maze[          i][MAX_COL-1-j].flood = 14-(i+j);
			maze[MAX_ROW-1-i][MAX_COL-1-j].flood = 14-(i+j);
		}

	InitStart(maze);
}

void InitStart(Maze maze)
{
	Position start;
	start.row = START_ROW;
	start.col = START_COL;
	start.dir = HERE;

	SetWall(maze, start, WALL,   NORTH);
	SetWall(maze, start, WALL,   EAST );
	SetWall(maze, start, WALL,   SOUTH);
	SetWall(maze, start, WALL,   WEST );
	SetWall(maze, start, NOWALL, START_DIR);
}

void Move(Position *pos, Direction dir)
{
	if(dir & NORTH)	pos->row--;
	if(dir & EAST)	pos->col++;
	if(dir & SOUTH)	pos->row++;
	if(dir & WEST)	pos->col--;
}

Direction LeftDir(Direction dir)
{ return (dir >> 1) | ((dir & NORTH) << 3); }

Direction RightDir(Direction dir)
{ return ((dir & NOT_WEST) << 1) | ((dir & WEST) >> 3); }

Direction BackDir(Direction dir)
{ return (dir >> 2) | ((dir & NORTH_EAST) << 2); }

//#define FF1
void FloodFill(Maze maze, Position endPos, Position startPos)
{
#ifdef FF1

	int i, j, k = 0;

	for(i = 0; i < MAX_ROW; i++)
		for(j = 0; j < MAX_COL; j++)
			maze[i][j].flood = MAX_FLOOD;

	// If flooding from center cell(s)
	if(startPos.row == -1 && startPos.col == -1)
	{
		for(i = 0; i <= 1; i++)
			for(j = 0; j <= 1; j++)
			{
				startPos.row = (MAX_ROW - i) / 2;
				startPos.col = (MAX_COL - j) / 2;
				maze[startPos.row][startPos.col].flood = 0;
			}
	}
	else
		maze[startPos.row][startPos.col].flood = k;

	while(GetFlood(maze, endPos, HERE) == MAX_FLOOD)
	{
		for(i = 0; i < MAX_ROW; i++)
			for(j = 0; j < MAX_COL; j++)
			{
				if(maze[i][j].flood == k)
				{
					UpdateFlood(maze, MakePos(i,j,NORTH));
					UpdateFlood(maze, MakePos(i,j,EAST));
					UpdateFlood(maze, MakePos(i,j,SOUTH));
					UpdateFlood(maze, MakePos(i,j,WEST));
				}
			}
		k++;
	}

/*
	List floodQueue;
	Position temp;
	int row, col;

	InitList(&floodQueue);
	PushBack(&floodQueue, endPos);

	GetCell(maze, endPos)->flood = MAX_FLOOD;

	while(GetFlood(maze, endPos, HERE) == MAX_FLOOD)
	{
		FloodQueue(maze, &floodQueue);
	}
*/

#else

	int i, j, k = 0;
	Position pos = {0, 0, HERE};

	for(pos.row = 0; pos.row < MAX_ROW; pos.row++)
		for(pos.col = 0; pos.col < MAX_COL; pos.col++)
			GetCell(maze, pos)->flood = MAX_FLOOD;

	// If flooding from center cell(s)
	if(startPos.row == -1 && startPos.col == -1)
	{
		for(i = 0; i <= 1; i++)
			for(j = 0; j <= 1; j++)
			{
				startPos.row = (MAX_ROW - i) / 2;
				startPos.col = (MAX_COL - j) / 2;
				maze[startPos.row][startPos.col].flood = 0;
			}
	}
	else
		maze[startPos.row][startPos.col].flood = k;

	while(GetFlood(maze, endPos, HERE) == MAX_FLOOD)
	{
		for(pos.row = 0; pos.row < MAX_ROW; pos.row++)
			for(pos.col = 0; pos.col < MAX_COL; pos.col++)
				if(GetFlood(maze, pos, HERE) == k)
					UpdateFlood(maze, pos);
		k++;
	}
#endif
}

void UpdateFlood(Maze maze, Position pos)
{
	const Position here = {pos.row, pos.col, HERE};
	const char hereFlood = GetFlood(maze, here, HERE);
	Direction dir;

	for(dir = NORTH; dir <= WEST; dir = dir << 1)
		if(!GetWall(maze,here,dir) && GetFlood(maze,here,dir) == MAX_FLOOD)
			GetCell(maze,MakePos(pos.row,pos.col,dir))->flood = hereFlood + 1;
}

void UpdateFlood2(Maze maze, Position pos, unsigned char floodValue)
{
	// Check if there's a wall between cell flooding to and from
	if(GetWall(maze,pos,pos.dir))
		return;

	if(GetFlood(maze,pos,pos.dir) == MAX_FLOOD)
		GetCell(maze,pos)->flood = floodValue;
		//maze[row][col].flood = floodValue;
}

void FloodQueue(Maze maze, List *floodQueue)
{
	Position cur = PopFront(floodQueue), temp;
	Direction checkDir;
	const int hereFlood = GetFlood(maze,cur,NORTH);
	int minFlood = hereFlood, curFlood;
	cur.dir = HERE;

	for(checkDir = NORTH; checkDir <= WEST; checkDir = RightDir(checkDir))
	{
		if(!GetWall(maze,cur,checkDir))
		{
			curFlood = GetFlood(maze,cur,checkDir);
			if(curFlood >= hereFlood && curFlood != MAX_FLOOD)
			{
				temp = cur;
				Move(&temp, checkDir);
				PushBack(floodQueue, temp);
			}
			else if(curFlood < minFlood)
				minFlood = curFlood;
		}
	}

/*
	if(!GetWall(maze,cur,NORTH))
		if(GetFlood(maze,cur,NORTH) >= hereFlood)
		{
			PushBack(floodQueue, MakePos(cur.row, cur.col, cur.dir));

		}
		else
		{


		}
*/


}

Command GetCmd(Maze maze, Position pos, Position goal)
{
	const int hereFlood = GetFlood(maze,pos,HERE);
	int minFlood = hereFlood;
	const Position temp = {pos.row, pos.col, HERE};
	const Direction forward = pos.dir;
	const Direction right = RightDir(forward);
	const Direction left = LeftDir(forward);
	const Direction back = BackDir(forward);
	Direction bestDir = HERE;

	bestDir = GetFloodDir(maze, pos, false);

	if(bestDir == forward)
		return FORWARD;
	if(bestDir == right)
		return RIGHT90;
	if(bestDir == left)
		return LEFT90;

	FloodFill(maze, pos, goal);
	bestDir = GetFloodDir(maze, pos, false);
	if(bestDir == forward)
		return FORWARD;
	if(bestDir == right)
		return RIGHT90;
	if(bestDir == left)
		return LEFT90;
	return STOP;
}

Direction GetFloodDir(Maze maze, Position pos, Bool diagonal)
{
	const int hereFlood = GetFlood(maze,pos,HERE);
	int minFlood = hereFlood;
	const Position temp = {pos.row, pos.col, HERE};
	const Direction forward = pos.dir;
	const Direction right = RightDir(forward);
	const Direction left = LeftDir(forward);
	const Direction back = BackDir(forward);
	Direction bestDir = HERE;

	if(GetFlood(maze,temp,forward) < minFlood && !GetWall(maze,temp,forward))
	{
		minFlood = GetFlood(maze,temp,forward);
		bestDir = forward;
	}
	if(GetFlood(maze,temp,right) < minFlood && !GetWall(maze,temp,right))
	{
		minFlood = GetFlood(maze,temp,right);
		bestDir = right;
	}
	if(GetFlood(maze,temp,left) < minFlood && !GetWall(maze,temp,left))
	{
		minFlood = GetFlood(maze,temp,left);
		bestDir = left;
	}
	return bestDir;
}


Cell *GetCell(Maze maze, Position pos)
{
	if(!ValidPos(pos))
		return NULL;

	Move(&pos, pos.dir);
	return &maze[pos.row][pos.col];
}

Wall GetWall(Maze maze, Position pos, Direction wall)
{
	Cell *cell;

	Move(&pos, pos.dir);

	if(wall==EAST  && pos.col==MAX_COL-1) return WALL;
	if(wall==SOUTH && pos.row==MAX_ROW-1) return WALL;

	if(wall & SOUTH_EAST) Move(&pos, wall);

	cell = GetCell(maze, pos);
	if(cell == NULL) return ERROR;

	switch(wall)
	{
		case NORTH:
		case SOUTH:	return cell->north;
		case EAST:
		case WEST:	return cell->west;
	}
	return ERROR;
}

void SetWall(Maze maze, Position pos, Wall wall, Direction wallDir)
{
	Cell *cell;

	if(wallDir & SOUTH_EAST) Move(&pos, wallDir);
	cell = GetCell(maze, pos);

	if(cell == NULL) return; // Check if cell is valid

	switch(wallDir)
	{
		case NORTH:
		case SOUTH:	cell->north_mapped = MAPPED;
				cell->north = wall;
				break;
		case EAST:
		case WEST:	cell->west_mapped = MAPPED;
				cell->west = wall;
				break;
	}

}

Position MakePos(int row, int col, Direction dir)
{
	Position pos = {row, col, dir};
	return pos;
}

int GetFlood(Maze maze, Position pos, Direction cellDir)
{
	pos.dir = cellDir;
	Cell *cell = GetCell(maze, pos);
	if(cell == NULL) return MAX_FLOOD;
	return cell->flood;
}

Bool Middle(Position pos)
{
	int i, j;

	for(i = 0; i <= 1; i++)
		for(j = 0; j <= 1; j++)
			if((MAX_ROW - i) / 2 == pos.row && (MAX_COL - j) / 2 == pos.col)
				return true;
	return false;
}


Bool ValidPos(Position pos)
{
	Move(&pos, pos.dir);
	return 0 <= pos.row && pos.row < MAX_ROW && 
	       0 <= pos.col && pos.col < MAX_COL ? true : false;
}

#ifdef DISPLAY
void GenerateBinaryMaze(Maze maze)
{
	Position pos = {0, 0, HERE};
	Wall wall;
	srand(time(0));

	for(pos.row = 0; pos.row < MAX_ROW; pos.row++)
		SetWall(maze, pos, WALL, WEST);

	for(pos.col = 0; pos.col < MAX_COL; pos.col++)
		SetWall(maze, pos, WALL, NORTH);

	for(pos.row = 1; pos.row < MAX_ROW; pos.row++)
		for(pos.col = 1; pos.col < MAX_COL; pos.col++)
		{
			wall = rand() % 2;
			SetWall(maze, pos, wall, WEST);
			SetWall(maze, pos, 1 - wall, NORTH);
		}

	InitStart(maze);
}
#endif

#ifdef DISPLAY
void GenerateMaze(Maze maze)
{
	List posList, dirList;
	Position newPos;
	int count, row, col, randEntry = 0;
	newPos.dir = HERE;

	InitList(&posList);
	InitList(&dirList);

	for(newPos.row = 0; newPos.row < MAX_ROW; newPos.row++)
		for(newPos.col = 0; newPos.col < MAX_COL; newPos.col++)
		{
			SetWall(maze, newPos, WALL, NORTH);
			SetWall(maze, newPos, WALL, WEST);
			maze[newPos.row][newPos.col].visited = UNMAPPED;
		}

	SetWall(maze, MakePos(7,7,HERE), NOWALL, SOUTH);
	SetWall(maze, MakePos(7,7,HERE), NOWALL, EAST);
	SetWall(maze, MakePos(8,8,HERE), NOWALL, NORTH);
	SetWall(maze, MakePos(8,8,HERE), NOWALL, WEST);
	SetWall(maze, MakePos(START_CELL,HERE), NOWALL, START_DIR);

	maze[7][7].visited = MAPPED;
	maze[7][8].visited = MAPPED;
	maze[8][7].visited = MAPPED;
	maze[8][8].visited = MAPPED;
	maze[START_ROW][START_COL].visited = MAPPED;

	count = 5;

	newPos = MakePos(7+rand()%2, 7+rand()%2, HERE);
	PushBack(&posList, newPos);

	while(GetSize(&posList))
	{
		DestroyList(&dirList);
		row = newPos.row;
		col = newPos.col;


		randEntry = rand() % GetSize(&posList);
		//randEntry = GetSize(&posList) - 1;
		newPos = GetList(&posList, randEntry);

		if(GetCell(maze, MakePos(row,col,NORTH)) != NULL)
			if(!(GetCell(maze, MakePos(row,col,NORTH))->visited))
				PushBack(&dirList, MakePos(0,0,NORTH));

		if(GetCell(maze, MakePos(row,col,EAST)) != NULL)
			if(!(GetCell(maze, MakePos(row,col,EAST))->visited))
				PushBack(&dirList, MakePos(0,0,EAST));

		if(GetCell(maze, MakePos(row,col,SOUTH)) != NULL)
			if(!(GetCell(maze, MakePos(row,col,SOUTH))->visited))
				PushBack(&dirList, MakePos(0,0,SOUTH));

		if(GetCell(maze, MakePos(row,col,WEST)) != NULL)
			if(!(GetCell(maze, MakePos(row,col,WEST))->visited))
				PushBack(&dirList, MakePos(0,0,WEST));

		if(GetSize(&dirList))
		{
			newPos = Remove(&dirList, rand()%GetSize(&dirList));
			newPos.row = row;
			newPos.col = col;
			SetWall(maze, MakePos(row,col,HERE), NOWALL, newPos.dir);
			GetCell(maze, MakePos(row,col,newPos.dir))->visited = MAPPED;
			Move(&newPos, newPos.dir);
			newPos.dir = HERE;
			PushBack(&posList, newPos);
			count++;
		}
		else
			Remove(&posList, randEntry);
	}
}
#endif

#ifdef DISPLAY
void DrawMaze(Maze maze)
{
	Position temp = {0, 0, HERE};
	char wall[] = "---", blank[] = "   ";

	for(temp.row = 0; temp.row < MAX_ROW; temp.row++)
	{
		for(temp.col = 0; temp.col < MAX_COL; temp.col++)
			printf("+%s", GetWall(maze, temp, NORTH) ? wall : blank);
		printf("+\n");
		for(temp.col = 0; temp.col < MAX_COL; temp.col++)
			if(GetFlood(maze, temp, HERE) == MAX_FLOOD)
				printf("%c   ", GetWall(maze, temp, WEST) ? '|' : ' ');
			else
				printf("%c%3d", GetWall(maze, temp, WEST) ? '|' : ' ', GetFlood(maze, temp, HERE));
		printf("|\n");
	}
	for(temp.col = 0; temp.col < MAX_COL; temp.col++)
		printf("+%s", wall);
	printf("+\n");
}
#endif

#ifdef DISPLAY
void DrawSim(Maze maze, Position pos)
{
	Position temp = {0, 0, HERE};
	char wall[] = "---", blank[] = "   ";

	for(temp.row = 0; temp.row < MAX_ROW; temp.row++)
	{
		for(temp.col = 0; temp.col < MAX_COL; temp.col++)
			printf("+%s", GetWall(maze, temp, NORTH) ? wall : blank);
		printf("+\n");
		for(temp.col = 0; temp.col < MAX_COL; temp.col++)
			if(pos.row == temp.row && pos.col == temp.col)
				printf("%cXXX", GetWall(maze, temp, WEST) ? '|' : ' ');
			else if(GetFlood(maze, temp, HERE) == MAX_FLOOD)
				printf("%c   ", GetWall(maze, temp, WEST) ? '|' : ' ');
			else
				printf("%c%3d", GetWall(maze, temp, WEST) ? '|' : ' ', GetFlood(maze, temp, HERE));
		printf("|\n");
	}
	for(temp.col = 0; temp.col < MAX_COL; temp.col++)
		printf("+%s", wall);
	printf("+\n");
}
#endif

#ifdef DISPLAY
void WritePos(Position pos)
{
	printf("%2d %2d ", pos.row, pos.col);
	if(pos.dir & NORTH) printf("N");
	if(pos.dir & EAST)  printf("E");
	if(pos.dir & SOUTH) printf("S");
	if(pos.dir & WEST)  printf("W");
	printf("\n");
}
#endif

#ifdef DISPLAY
void DisplayPos(Maze maze, Position pos)
{
	Position temp = {0, 0, HERE};
	char wall[] = "---", blank[] = "   ";
	WritePos(pos);

	for(temp.row = pos.row - 1; temp.row <= pos.row + 1; temp.row++)
	{
		for(temp.col = pos.col - 1; temp.col <= pos.col + 1; temp.col++)
			printf("+%s", GetWall(maze, temp, NORTH) ? wall : blank);
		printf("+\n");
		for(temp.col = pos.col - 1; temp.col <= pos.col + 1; temp.col++)
			printf("%c%3d", GetWall(maze, temp, WEST) ? '|' : ' ', GetFlood(maze, temp, HERE));
		printf("|\n");
	}
	for(temp.col = pos.col - 1; temp.col <= pos.col + 1; temp.col++)
		printf("+%s", GetWall(maze, temp, NORTH) ? wall : blank);
	printf("+\n");
}
#endif


