#include "MazeUtils.h"

void printMaze(Cell maze[maze_height][maze_width]){
    int i;
    int j;

    for(i = 0; i< MAZE_HEIGHT ; i++ )
    {
        for(j = 0; j < MAZE_WIDTH ; j++)
        {
            printf("%d   ", maze[i][j].floodValue);

        }
        printf("\n\n");
    }

}

void initMaze(Cell maze[maze_height][maze_width], char rawMaze[RAWH][RAWW])
{
    int rowCounter = 0;
    int columnCounter = 0;
    int x;
    int y;
    int i = 0;
    int j = 0;

    
    for(i = 0; i < MAZE_HEIGHT ; i++)
    {
        for( j = 0; j < MAZE_WIDTH ; j++)
        {
            maze[i][j].north = FALSE;
            maze[i][j].south = FALSE;
            maze[i][j].west = FALSE;
            maze[i][j].east = FALSE;
            maze[i][j].x = j;                                 // Initalize the X and Y
            maze[i][j].y = i;
            maze[i][j].floodValue = 0;
            // Initalize Flood Value
        }

    }



}
void grabMaze(Cell maze[maze_height][maze_width], char rawMaze[RAWH][RAWW])
{
    int rowCounter = 0;
    int columnCounter = 0;
    int x;
    int y;
    int i = 0;
    int j = 0;

    
    for(i = 0; i < MAZE_HEIGHT ; i++)
    {
        for( j = 0; j < MAZE_WIDTH ; j++)
        {
            maze[i][j].north = checkRawNorth(rawMaze, j, i);           // Check North
            maze[i][j].south = checkRawSouth(rawMaze, j, i);           // Check East
            maze[i][j].west = checkRawWest(rawMaze, j, i);             // Check West
            maze[i][j].east = checkRawEast(rawMaze, j, i);             // Check South
            maze[i][j].x = j;                                 // Initalize the X and Y
            maze[i][j].y = i;
            maze[i][j].floodValue = 0;
            // Initalize Flood Value

            

        }

    }



}
