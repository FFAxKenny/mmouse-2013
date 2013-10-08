#include "Maze.h"
#include "tfdef.h"

void Maze_print(Cell maze[maze_height][maze_width]){
    int i;
    int j;

    for(i = 0; i< MAZE_HEIGHT ; i++ )
    {
        for(j = 0; j < MAZE_WIDTH ; j++)
        {
            printf("%02d  ", maze[i][j].floodValue);

        }
        printf("\n\n");
    }

}

void Maze_init(Cell maze[maze_height][maze_width])
{
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

            if( (i == 7 || i == 8) && (j == 7 || j == 8 ) )
                maze[i][j].floodValue = 0;
            else
                maze[i][j].floodValue = -1;
               
        }

    }


}
void Maze_flood(Cell maze[maze_height][maze_width])
{
    int x;
    int y;
    int i = 0;
    int j = 0;
    

    // Go through each cell
    for(i = 0; i < MAZE_HEIGHT ; i++)
    {
        for( j = 0; j < MAZE_WIDTH ; j++)
        {
               
        }

    }


}




// Returns if the corresponding cell has been flooded yet
int Maze_checkCellNeighbor(Cell maze[maze_height][maze_width], int i, int j, int direction){
    switch(direction)
    {
        case NORTH: 
        {
            if( i == 0)
                return TRUE;
            else if(maze[i-1][j].floodValue == -1)
                return FALSE;
            else 
                return TRUE;
        case SOUTH:
            if( i == 15)
                return TRUE;
            else if(maze[i+1][j].floodValue == -1)
                return FALSE;
            else 
                return TRUE;
        case EAST:
            if( j == 15)
                return TRUE;
            else if(maze[i][j+1].floodValue == -1)
                return FALSE;
            else return TRUE;
        case WEST:
            if( j == 0)
                return TRUE;
            else if(maze[i][j-1].floodValue == -1)
                return FALSE;
            else 
                return TRUE;
        default:
            return FALSE;
        }
    }
}

void Maze_initFromRaw(Cell maze[maze_height][maze_width], char rawMaze[RAWH][RAWW])
{
    int x;
    int y;
    int i = 0;
    int j = 0;

    for(i = 0; i < MAZE_HEIGHT ; i++)
    {
        for( j = 0; j < MAZE_WIDTH ; j++)
        {
            maze[i][j].north = RawMaze_checkNorth(rawMaze,i,j);
            maze[i][j].south = RawMaze_checkSouth(rawMaze,i,j);
            maze[i][j].west = RawMaze_checkWest(rawMaze,i,j);
            maze[i][j].east = RawMaze_checkEast(rawMaze,i,j);
            maze[i][j].x = j;                             // Initalize the X and Y
            maze[i][j].y = i;

            if( (i == 7 || i == 8) && (j == 7 || j == 8 ) )
                maze[i][j].floodValue = 0;
            else
                maze[i][j].floodValue = -1;
        }

    }


}

void Maze_grab(Cell maze[maze_height][maze_width], char rawMaze[RAWH][RAWW]){
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
            maze[i][j].north = RawMaze_checkNorth(rawMaze, j, i);           // Check North
            maze[i][j].south = RawMaze_checkSouth(rawMaze, j, i);           // Check East
            maze[i][j].west = RawMaze_checkWest(rawMaze, j, i);             // Check West
            maze[i][j].east = RawMaze_checkEast(rawMaze, j, i);             // Check South
            maze[i][j].x = j;                                 // Initalize the X and Y
            maze[i][j].y = i;
            maze[i][j].floodValue = 0;
            // Initalize Flood Value

        }

    }



}
