#include <stdio.h>
#include "rawMazeUtils.h"
#include "tfdef.h"

int checkRawNorth(char rawmaze[rawH][rawW], int x, int y) {
    int rawX = mazeToRawX(x);
    int rawY = mazeToRawY(y);


    if( rawmaze[rawY-1][rawX] == '-')
        return TRUE;
    else
        return FALSE;
}
int checkRawSouth(char rawmaze[rawH][rawW], int x, int y) {
    int rawX = mazeToRawX(x);
    int rawY = mazeToRawY(y);


    if( rawmaze[rawY+1][rawX] == '-')
        return TRUE;
    else
        return FALSE;
}

int checkRawWest(char rawmaze[rawH][rawW], int x, int y) {
    int rawX = mazeToRawX(x);
    int rawY = mazeToRawY(y);


    if( rawmaze[rawY][rawX-2] == '|')
        return TRUE;
    else
        return FALSE;
}

int checkRawEast(char rawmaze[rawH][rawW], int x, int y) {
    int rawX = mazeToRawX(x);
    int rawY = mazeToRawY(y);
    if( rawmaze[rawY][rawX+2] == '|')
        return TRUE;
    else
        return FALSE;
}


void readRawMaze(char rawMaze[rawMaze_height][rawMaze_width])
{
    int x=0;
    int y=0;
    char input;
    
    input = getchar();
    while(input != EOF)
    {
        if(input == '\n'){
            y++;
            x = 0;
            printf("Row is now %d\n", y);
        }
        else{
            rawMaze[y][x] = input;
            printf("column is now %d\n", x);
            x++;
        }
        input = getchar();
    }

}
void printRawMaze(char rawMaze[rawMaze_height][rawMaze_width])
{
    int i,j;
    for( i = 0; i < 33; i++)
    {
        for( j = 0; j < 65 ; j++ )
        {
            printf("%c", rawMaze[i][j]);
        }
        printf("\n");
    }

}

int rawToMazeX(int rawX)        // Converts RAW x coordnates to Maze X coordinates
{
    return (rawX/2 - 1);

}
int rawToMazeY(int rawY)       // Concerts RAW Y coordinates to Maze Y coordinates
{
    return ((rawY -1)/2);
}

int mazeToRawX(int mazeX){
    int temp;
    temp = (mazeX*4) + 2;
    return temp;
}

int mazeToRawY(int mazeY)
{
    int temp;
    temp = ( mazeY*2 + 1);
    return temp;
}

void checkCell(char rawmaze[rawH][rawW], int x, int y)
{
    printf("North: ");
    if(checkRawNorth(rawmaze, x,y)) printf("X\n");
    else printf("\n");

    printf("East: ");
    if(checkRawEast(rawmaze, x,y)) printf("X\n");
    else printf("\n");

    printf("West: ");
    if(checkRawWest(rawmaze, x,y)) printf("X\n");
    else printf("\n");
    
    printf("South: ");
    if(checkRawSouth(rawmaze, x,y)) printf("X\n");
    else printf("\n");
}
