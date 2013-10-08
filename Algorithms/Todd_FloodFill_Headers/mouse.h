
#ifndef MOUSE_H
#define MOUSE_H

#include "sensor.h"
#include "motor.h"
#include "button.h"
#include "maze.h"

// Kellen Prototypes
void solve_maze(Maze *maze);
void self_destruct();

// --- Solving Commands ---
void Mouse_RunCommand(Command cmd, Maze *maze, int *myRow, int *myCol, Direction *myDir, bool map);
bool Mouse_SpeedRun(Maze *maze, int *myRow, int *myCol, Direction *myDir, uint16_t speed);
void Mouse_MapRL(Maze *maze, int myRow, int myCol, Direction myDir);
void Mouse_MapF(Maze *maze, int myRow, int myCol, Direction myDir);
void Mouse_TrackRL(Maze *maze, int myRow, int myCol, Direction myDir);
void Mouse_TrackF(Maze *maze, int myRow, int myCol, Direction myDir);

#endif
