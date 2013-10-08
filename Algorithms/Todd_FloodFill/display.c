
#include "display.h"

void GenerateBinaryMaze(Maze *maze)
{
	int i, j;

	Maze_Init(maze);

	srand(0);
	//srand(0);

	for(i = 1; i < MAX_ROW; i++)
		for(j = 1; j < MAX_COL; j++)
		{
			if(rand()%2)
				//maze.walls.[i] |= 1 << col;
				SetWallN(*maze,i,j,1);
			else
				//maze.walls.[i] |= 1 << col;
				SetWallW(*maze,i,j,1);
		}
}

void DisplayMaze(Maze maze, int x, int y)
{
	int i, j;
	char wall[] = "---", nowall[] = "   ", mouse[] = "XXX";

	// --- Draw Entire Maze ---
	for(i = 0; i < MAX_ROW; i++)
	{
		// --- Display North Walls ---
		for(j = 0; j < MAX_COL; j++)
			printf("+%s",GetWallN(maze,i,j) ? wall : nowall);
		printf("+\n");

		// --- Display West Walls ---
		for(j = 0; j < MAX_COL; j++)
			if(i == x && j == y)
				printf("%s%s",GetWallW(maze,i,j) ? "|" : " ", mouse);
			else if(maze.cell[i][j].flood == MAX_FLOOD)
				printf("%s   ",GetWallW(maze,i,j) ? "|" : " ");
			else
				printf("%s%3d",GetWallW(maze,i,j) ? "|" : " ", maze.cell[i][j].flood);
		printf("%s\n",GetWallE(maze,i,MAX_COL-1) ? "|" : " ");
	}

	// --- Draw Last Row ---
	for(j = 0; j < MAX_COL; j++)
		printf("+%s",GetWallS(maze,MAX_ROW-1,j) ? wall : nowall);
	printf("+\n");

}

void DisplayMazeCmd(Maze maze)
{
	int i, j;
	char wall[] = "---", nowall[] = "   ";
	char forward[] = " F ", right[] = " R ", left[] = " L ", stop[] = "   ";
	char north[] = " ^ ", east[] = " > ", south[] = " v ", west[] = " < ";

	// --- Draw Entire Maze ---
	for(i = 0; i < MAX_ROW; i++)
	{
		// --- Display North Walls ---
		for(j = 0; j < MAX_COL; j++)
			printf("+%s",GetWallN(maze,i,j) ? wall : nowall);
		printf("+\n");

		// --- Display West Walls ---
		for(j = 0; j < MAX_COL; j++)
		{
			printf("%s",GetWallW(maze,i,j) ? "|" : " ");
			switch(maze.cell[i][j].cmd)
			{
				case FORWARD: printf("%s", forward); break;
				case STOP   : printf("%s",    stop); break;
				case RIGHT90: printf("%s",   right); break;
				case LEFT90 : printf("%s",    left); break;
			}
		}

		printf("%s\n",GetWallE(maze,i,MAX_COL-1) ? "|" : " ");
	}

	// --- Draw Last Row ---
	for(j = 0; j < MAX_COL; j++)
		printf("+%s",GetWallS(maze,MAX_ROW-1,j) ? wall : nowall);
	printf("+\n");

}

