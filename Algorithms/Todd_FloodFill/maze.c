
#include "maze.h"

void Maze_Init(Maze *maze)
{
	// --- Counter Variables ---
	int i, j;

	// --- Clear Wall Array ---
	for(i = 0; i < WALL_ARRAY; i++)
	{
		maze->walls.walls[i]  = NOWALL;
		maze->mapped.walls[i] = UNMAPPED;
	}

	// --- Initialize Maze Boundaries ---
	maze->walls.top     = 0xFFFF;
	maze->walls.right   = 0xFFFF;
	maze->walls.left    = 0xFFFF;
	maze->walls.bottom  = 0xFFFF;
	maze->mapped.top    = 0xFFFF;
	maze->mapped.right  = 0xFFFF;
	maze->mapped.left   = 0xFFFF;
	maze->mapped.bottom = 0xFFFF;

	// --- Initialize Flood Values ---
	for(i = 0; i < 8; i++)
	{
		for(j = 0; j < 8; j++)
		{
			maze->cell[   i][   j].flood = 14-i-j;
			maze->cell[15-i][   j].flood = 14-i-j;
			maze->cell[   i][15-j].flood = 14-i-j;
			maze->cell[15-i][15-j].flood = 14-i-j;
		}
	}

	// --- Initialize Commands ---
	for(i = 0; i < MAX_ROW; i++)
		for(j = 0; j < MAX_COL; j++)
			maze->cell[i][j].cmd = STOP;
}

Command Maze_GetCmd(Maze *maze, int myRow, int myCol, Direction myDir, int endRow, int endCol)
{
	// --- Current Flood Variable ---
	int myFlood = maze->cell[myRow][myCol].flood;

	// --- Check for Cells with Lower Flood ---
	if(Maze_GetFlood(maze,myRow,myCol,         myDir ) < myFlood) return FORWARD;
	if(Maze_GetFlood(maze,myRow,myCol,RightDir(myDir)) < myFlood) return RIGHT90;
	if(Maze_GetFlood(maze,myRow,myCol, LeftDir(myDir)) < myFlood) return LEFT90;
	if(Maze_GetFlood(maze,myRow,myCol, BackDir(myDir)) < myFlood) return STOP;

	// --- Reflood and Get Command from Updated Maze ---
	Maze_Floodfill(maze, myRow, myCol, endRow, endCol, UNMAPPED);
	return Maze_GetCmd(maze, myRow, myCol, myDir, endRow, endCol);
}

void Maze_SetCmd(Maze *maze, int myRow, int myCol, Direction myDir)
{
	// --- Variable Declaration ---
	int i, j;      // Counter Variables
	Command myCmd; // Command Variable

	// --- Initialize Flood Values ---
	Maze_Floodfill(maze, myRow, myCol, -1, -1, MAPPED);

	// --- Initialize Commands ---
	for(i = 0; i < MAX_ROW; i++)
		for(j = 0; j < MAX_COL; j++)
			maze->cell[i][j].cmd = STOP;

	// --- Map Path to the Center ---
	while(!CenterPos(myRow,myCol))
	{
		// --- Get and Execute Command ---
		myCmd = Maze_GetCmd(maze, myRow, myCol, myDir, -1, -1);
		switch(myCmd)
		{
			case FORWARD:                          break;
			case STOP:    myDir =  BackDir(myDir); break;
			case RIGHT90: myDir = RightDir(myDir); break;
			case LEFT90:  myDir =  LeftDir(myDir); break;
		}
		maze->cell[myRow][myCol].cmd = myCmd;
		Move(myRow, myCol, myDir);
	}
}

void Maze_MapWall(Maze *maze, int myRow, int myCol, Direction wallDir, Wall wall, Mapped remap)
{
	if(remap == MAPPED || Maze_GetMapped(maze,myRow,myCol,wallDir) == UNMAPPED)
		Maze_SetWall(maze,myRow,myCol,wallDir,wall);
}

Wall Maze_GetWall(Maze *maze, int myRow, int myCol, Direction wallDir)
{
	switch(wallDir)
	{
		case NORTH: return GetWallN(*maze,myRow,myCol);
		case  EAST: return GetWallE(*maze,myRow,myCol);
		case SOUTH: return GetWallS(*maze,myRow,myCol);
		case  WEST: return GetWallW(*maze,myRow,myCol);
	}
}

Mapped Maze_GetMapped(Maze *maze, int myRow, int myCol, Direction wallDir)
{
	switch(wallDir)
	{
		case NORTH: return GetMappedN(*maze,myRow,myCol);
		case  EAST: return GetMappedE(*maze,myRow,myCol);
		case SOUTH: return GetMappedS(*maze,myRow,myCol);
		case  WEST: return GetMappedW(*maze,myRow,myCol);
	}
}

void Maze_SetWall(Maze *maze, int myRow, int myCol, Direction wallDir, Wall wall)
{
	switch(wallDir)
	{
		case NORTH:
			SetWallN(*maze,myRow,myCol,wall);
			MapWallN(*maze,myRow,myCol);
			break;

		case EAST:
			SetWallE(*maze,myRow,myCol,wall);
			MapWallE(*maze,myRow,myCol);
			break;

		case SOUTH:
			SetWallS(*maze,myRow,myCol,wall);
			MapWallS(*maze,myRow,myCol);
			break;

		case WEST:
			SetWallW(*maze,myRow,myCol,wall);
			MapWallW(*maze,myRow,myCol);
			break;
	}
}

uint16_t Maze_GetFlood(Maze *maze, int row, int col, Direction dir)
{
	Wall wall;
	switch(dir)
	{
		case HERE : wall = NOWALL;                  break;
		case NORTH: wall = GetWallN(*maze,row,col); break;
		case EAST : wall = GetWallE(*maze,row,col); break;
		case SOUTH: wall = GetWallS(*maze,row,col); break;
		case WEST : wall = GetWallW(*maze,row,col); break;
	}
	if(wall) return MAX_FLOOD - 1;

	Move(row,col,dir);
	if(ValidPos(row,col))
		return maze->cell[row][col].flood;
	return MAX_FLOOD - 1;
}

void Maze_Floodfill(Maze *maze, int endRow, int endCol, int startRow, int startCol, Mapped check)
{
	// --- Static Variables ---
	const static int
	midRow1 = 7, midCol1 = 7, // North West Center Cell
	midRow2 = 7, midCol2 = 8, // North East Center Cell
	midRow3 = 8, midCol3 = 7, // South West Center Cell
	midRow4 = 8, midCol4 = 8; // South East Center Cell

	// --- Floodfill Variables ---
	int i, j;          // Counters
	int row, col;      // Current Cell
	uint8_t hereFlood; // Current Cell Flood Value
	Queue q;           // Floodfill Queue

	// --- Initialize Queue ---
	InitQueue(&q);

	// --- Initialize Maze ---
	for(i = 0; i < MAX_ROW; i++)
		for(j = 0; j < MAX_COL; j++)
			maze->cell[i][j].flood = MAX_FLOOD;
	
	// --- Initialize Maze and Queue ---
	if(startRow == -1 &&  startCol == -1)
	{
		// Initialize Center Cells' Flood Values
		maze->cell[midRow1][midCol1].flood = 0;
		maze->cell[midRow2][midCol2].flood = 0;
		maze->cell[midRow3][midCol3].flood = 0;
		maze->cell[midRow4][midCol4].flood = 0;

		// Put Center Cells in Queue
		Push(&q, (midRow1 << 8) + midCol1);
		Push(&q, (midRow2 << 8) + midCol2);
		Push(&q, (midRow3 << 8) + midCol3);
		Push(&q, (midRow4 << 8) + midCol4);
	}
	else
	{
		// Initialize Destination Cell's Flood Values
		maze->cell[startRow][startCol].flood = 0;
		Push(&q, (startRow << 8) + startCol);
	}

	// --- Floodfill ---
	while(maze->cell[endRow][endCol].flood == MAX_FLOOD)
	{
		// --- Get Current Cell from Queue ---
		row = Front(&q) >> 8;
		col =   Pop(&q)  & 0xFF;
		hereFlood =  maze->cell[row][col].flood;

		// --- Check North ---
		if(GetMappedN(*maze,row,col) || GetMappedN(*maze,row,col) == check)
			if(Maze_GetFlood(maze,row,col,NORTH) == MAX_FLOOD)
			{
				maze->cell[row-1][col].flood =  hereFlood + 1;
				Push(&q, (row - 1 << 8) + col);
			}

		// --- Check East ---
		if(GetMappedE(*maze,row,col) || GetMappedE(*maze,row,col) == check)
			if(Maze_GetFlood(maze,row,col,EAST) == MAX_FLOOD)
			{
				maze->cell[row][col+1].flood =  hereFlood + 1;
				Push(&q, (row << 8) + (col + 1));
			}

		// --- Check South ---
		if(GetMappedS(*maze,row,col) || GetMappedS(*maze,row,col) == check)
			if(Maze_GetFlood(maze,row,col,SOUTH) == MAX_FLOOD)
			{
				maze->cell[row+1][col].flood =  hereFlood + 1;
				Push(&q, (row + 1 << 8) + col);
			}

		// --- Check West ---
		if(GetMappedW(*maze,row,col) || GetMappedW(*maze,row,col) == check)
			if(Maze_GetFlood(maze,row,col,WEST) == MAX_FLOOD)
			{
				maze->cell[row][col-1].flood =  hereFlood + 1;
				Push(&q, (row << 8) + col - 1);
			}
	}
}

void Maze_FloodfillGP(Maze *maze, int endRow, int endCol, int startRow, int startCol)
{
	// --- Static Variables ---
	const static int
		midRow1 = 7, midCol1 = 7,
		midRow2 = 7, midCol2 = 8,
		midRow3 = 8, midCol3 = 7,
		midRow4 = 8, midCol4 = 8;

	// --- Floodfill Variables ---
	int i, j;          // Counters
	int row, col, pos; // Current Cell
	uint8_t hereFlood; // Current Cell Flood Value
	Heap h;           // Floodfill Priority Queue

	Heap_Init(&h);

	for(i = 0; i < MAX_ROW; i++)
		for(j = 0; j < MAX_COL; j++)
			maze->cell[i][j].flood = MAX_FLOOD;

	// --- Initialize Maze and Queue ---
	if(startRow == -1 &&  startCol == -1)
	{
		// Initialize Center Cells' Flood Values
		maze->cell[midRow1][midCol1].flood = 0;
		maze->cell[midRow2][midCol2].flood = 0;
		maze->cell[midRow3][midCol3].flood = 0;
		maze->cell[midRow4][midCol4].flood = 0;

		Heap_Insert(&h, (midRow1<<4)+midCol1, 0);
		Heap_Insert(&h, (midRow2<<4)+midCol2, 0);
		Heap_Insert(&h, (midRow3<<4)+midCol3, 0);
		Heap_Insert(&h, (midRow4<<4)+midCol4, 0);
	}
	else
	{
		// Initialize Destination Cell's Flood Values
		maze->cell[startRow][startCol].flood = 0;
		Heap_Insert(&h, (startRow<<4)+startCol, 0);
	}
	
	while(maze->cell[endRow][endCol].flood == MAX_FLOOD)
	{
		// Get Current Cell from Queue
		pos       = Heap_Remove(&h);
		row       = pos >> 4;
		col       = pos  & 0xF;
		hereFlood = maze->cell[row][col].flood;

//printf("%d %d\n",row,col);

		// === CHECK NORTH ===
		if(row > 0 && !GetWallN(*maze,row,col))
			if(maze->cell[row-1][col].flood > hereFlood)
			{
				// Check South for Straight Path
				if(row < MAX_ROW - 1 && !GetWallS(*maze,row,col))
					//if(maze->flood[row+1][col] <= hereFlood)
						maze->cell[row-1][col].flood =  hereFlood + FLOOD_0;
				else
					maze->cell[row-1][col].flood =  hereFlood + FLOOD_90;
				Heap_Insert(&h, (row-1<<4)+col, hereFlood);
			}

		// Check East
		if(col < MAX_COL - 1 && !GetWallE(*maze,row,col))
			if(maze->cell[row][col+1].flood > hereFlood)
			{
				//if(maze->flood[row][col-1] <= hereFlood)
				if(col > 0 && !GetWallW(*maze,row,col))
					maze->cell[row][col+1].flood =  hereFlood + FLOOD_0;
				else
					maze->cell[row][col+1].flood =  hereFlood + FLOOD_90;
				Heap_Insert(&h, (row<<4)+(col+1), hereFlood);

			}

		// Check South
		if(row < MAX_ROW - 1 && !GetWallS(*maze,row,col))
			if(maze->cell[row+1][col].flood > hereFlood)
			{
				if(row > 0  && !GetWallN(*maze,row,col))
					maze->cell[row+1][col].flood =  hereFlood + FLOOD_0;
				else
					maze->cell[row+1][col].flood =  hereFlood + FLOOD_90;
				Heap_Insert(&h, (row+1<<4)+col, hereFlood);
			}

		// Check West
		if(col > 0 && !GetWallW(*maze,row,col))
			if(maze->cell[row][col-1].flood > hereFlood)
			{
				if(col < MAX_COL - 1 && !GetWallE(*maze,row,col))
					maze->cell[row][col-1].flood =  hereFlood + FLOOD_0;
				else
					maze->cell[row][col-1].flood =  hereFlood + FLOOD_90;
				Heap_Insert(&h, (row<<4)+col-1, hereFlood);
			}

//DisplayMaze(*maze, 15, 0);
//getchar();

//printf("row: %d, col: %d, flood: %d\n", row, col, hereFlood);
//Traverse(&q,PrintPos);
/*
DisplayMaze(*maze, 15, 0);
printf("row: %d, col: %d, flood: %d\n", row, col, hereFlood);
Traverse(&q,PrintPos);
getchar();
*/

	}

}

