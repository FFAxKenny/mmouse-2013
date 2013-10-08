#ifndef MAZE_H
#define MAZE_H

#define ERROR -1

#define DISPLAY

#ifdef DISPLAY
#include <stdio.h>
#include <time.h>
#endif

#include "common.h"
#include "list.h"

/*--------------------------------*
 *    MAZE FUNCTION PROTOTYPES    *
 *--------------------------------*/
void InitMaze(Maze maze);
void InitStart(Maze maze);

/*------------------------*
 *    MUTATOR FUNCTIONS   *
 *------------------------*/
void Move(Position *pos, Direction dir);
Direction LeftDir (Direction dir);
Direction RightDir(Direction dir);
Direction BackDir (Direction dir);
void FloodFill(Maze maze, Position endPos, Position startPos);
void UpdateFlood(Maze maze, Position pos);
void UpdateFlood2(Maze maze, Position pos, unsigned char floodValue);
void FloodQueue(Maze maze, List *floodQueue);
void SetWall(Maze maze, Position pos, Wall wall, Direction wallDir);
Position MakePos(int row, int col, Direction dir);

/*-------------------------*
 *    ACCESSOR FUNCTIONS   *
 *-------------------------*/
Command GetCmd(Maze maze, Position pos, Position goal);
Direction GetFloodDir(Maze maze, Position pos, Bool diagonal);
Cell *GetCell(Maze maze, Position pos);
Wall GetWall(Maze maze, Position pos, Direction wall);
int GetFlood(Maze maze, Position pos, Direction cellDir);

/*----------------------*
 *    CHECK FUNCTIONS   *
 *----------------------*/
Bool Middle(Position pos);
Bool ValidPos(Position pos);

/*------------------------*
 *    DISPLAY FUNCTIONS   *
 *------------------------*/
#ifdef DISPLAY
void GenerateBinaryMaze(Maze maze);
void GenerateMaze(Maze maze);
void DrawMaze(Maze maze);
void DrawSim(Maze maze, Position pos);
void WritePos(Position pos);
void DisplayPos(Maze maze, Position pos);
#endif

#endif

