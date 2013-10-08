#include <stdio.h>
#include "RawMaze.h"
#include "tfdef.h"

int RawMaze_checkNorth(char rawmaze[rawH][rawW], int x, int y) {
    int rawX = RawMaze_mazeToRawX(x);
    int rawY = RawMaze_mazeToRawY(y);

    if( rawmaze[rawY-1][rawX] == '-')
        return TRUE;
    else
        return FALSE;
}
int RawMaze_checkSouth(char rawmaze[rawH][rawW], int x, int y) {
    int rawX = RawMaze_mazeToRawX(x);
    int rawY = RawMaze_mazeToRawY(y);


    if( rawmaze[rawY+1][rawX] == '-')
        return TRUE;
    else
        return FALSE;
}

int RawMaze_checkWest(char rawmaze[rawH][rawW], int x, int y) {
    int rawX = RawMaze_mazeToRawX(x);
    int rawY = RawMaze_mazeToRawY(y);


    if( rawmaze[rawY][rawX-2] == '|')
        return TRUE;
    else
        return FALSE;
}

int RawMaze_checkEast(char rawmaze[rawH][rawW], int x, int y) {
    int rawX = RawMaze_mazeToRawX(x);
    int rawY = RawMaze_mazeToRawY(y);
    if( rawmaze[rawY][rawX+2] == '|')
        return TRUE;
    else
        return FALSE;
}


void RawMaze_read(char rawMaze[rawMaze_height][rawMaze_width])
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
void RawMaze_print(char rawMaze[rawMaze_height][rawMaze_width])
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

int RawMaze_rawToMazeX(int rawX)        // Converts RAW x coordnates to Maze X coordinates
{
    return (rawX/2 - 1);

}
int RawMaze_rawToMazeY(int rawY)       // Concerts RAW Y coordinates to Maze Y coordinates
{
    return ((rawY -1)/2);
}

int RawMaze_mazeToRawX(int mazeX){
    int temp;
    temp = (mazeX*4) + 2;
    return temp;
}

int RawMaze_mazeToRawY(int mazeY)
{
    int temp;
    temp = ( mazeY*2 + 1);
    return temp;
}

void RawMaze_checkCell(char rawmaze[rawH][rawW], int x, int y)
{
    printf("North: ");
    if( RawMaze_checkNorth(rawmaze, x,y) ) printf("X\n"); 
    else printf("\n");

    printf("East: ");
    if ( RawMaze_checkEast(rawmaze, x,y) ) printf("X\n");
    else printf("\n");

    printf("West: ");
    if ( RawMaze_checkWest(rawmaze, x,y) )printf("X\n");
    else printf("\n");
    
    printf("South: ");
    if (RawMaze_checkSouth(rawmaze, x,y) )printf("X\n");
    else printf("\n");
}
