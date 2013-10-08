#ifndef MAZE_H
#define MAZE_H

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "queue.h"
#include "button.h"

// =================
//    MAZE MACROS
// =================

// --- Maze Size ---
#define MAX_ROW    16
#define MAX_COL    16
#define WALL_ARRAY (MAX_ROW + MAX_COL + 2)

// --- Initial Position ---
#define START_ROW  15
#define START_COL  0
#define START_DIR  NORTH

// --- Floodfill Parameters ---
#define MAX_FLOOD  0x1FFF
#define FLOOD_0  4
#define FLOOD_45 6
#define FLOOD_90 8


// =======================
//    MAZE ENUMERATIONS
// =======================

// --- Direction Data Type ---
typedef enum
{
	HERE  = 0b0000,
	ALL   = 0b1111,

	NORTH = 0b0001,
	EAST  = 0b0010,
	SOUTH = 0b0100,
	WEST  = 0b1000,

	NORTH_EAST = NORTH | EAST,
	NORTH_WEST = NORTH | WEST,
	SOUTH_EAST = SOUTH | EAST,
	SOUTH_WEST = SOUTH | WEST,

	NOT_NORTH = ALL ^ NORTH,
	NOT_EAST  = ALL ^  EAST,
	NOT_SOUTH = ALL ^ SOUTH,
	NOT_WEST  = ALL ^  WEST
}
Direction;

// --- Command Data Type ---
typedef enum
{
	// Name = Bits   // Bit2    | Bit1   | Bit0   |
	STOP    = 0b000, // Forward | Slower | Stop   |
	SLOWER  = 0b001, // Forward | Slower | Slower |
	FORWARD = 0b010, // Forward | Faster | Same   |
	FASTER  = 0b011, // Forward | Faster | Faster |
	LEFT45  = 0b100, // Turn    | Left   | 45 deg |
	LEFT90  = 0b101, // Turn    | Left   | 90 deg |
	RIGHT45 = 0b110, // Turn    | Right  | 45 deg |
	RIGHT90 = 0b111  // Turn    | Right  | 90 deg |
}
Command;

// --- Wall Data Type ---
typedef enum
{
	WALL    = true,
	NOWALL  = false
}
Wall;

// --- Mapped Data Type ---
typedef enum
{
	MAPPED   = true,
	UNMAPPED = false
}
Mapped;

// --- Walls Data Type ---
typedef union
{
	uint16_t walls[WALL_ARRAY];
	struct
	{
		uint16_t n[MAX_ROW]; // Bits for All North Walls
		uint16_t bottom;     // South Walls of Last Row
		uint16_t w[MAX_COL]; // Bits for All West Walls
		uint16_t right;      // East Walls of Last Column
	};
	struct
	{
		uint16_t top;        // North Walls of First Row
		uint16_t s[MAX_ROW]; // Bits for All North Walls
		uint16_t left;       // East Walls of Last Column
		uint16_t e[MAX_COL]; // Bits for All West Walls
	};
}
Walls;

// --- Cell Data Type ---
typedef struct
{
	uint16_t cmd: 3;
	uint16_t flood: 13;
}
Cell;

// --- Cells Data Type ---
typedef Cell Cells[MAX_ROW][MAX_COL];

// --- Maze Data Type ---
typedef struct
{
	Cells cell;
	Walls walls;
	Walls mapped;
}
Maze;


// ===============================
//    POSITION MACRO FUNCTIONS
// ===============================

// --- Boolean Functions ---
#define ValidPos(row,col)  (0<=row && row<MAX_ROW && 0<=col && col<MAX_COL)
#define CenterPos(row,col) ((row==7 || row==8) && (col==7 || col==8))
#define StartPos(row,col)  (row==START_ROW && col==START_COL)

// --- Mutator Functions ---
#define Move(row,col,dir) {row += South(dir) - North(dir); \
                           col +=  East(dir) -  West(dir);}

// ===============================
//    DIRECTION MACRO FUNCTIONS
// ===============================

// --- Boolean Functions ---
#define North(dir) (dir      & 1) // Returns 1 if dir == NORTH
#define  East(dir) (dir >> 1 & 1) // Returns 1 if dir ==  EAST
#define South(dir) (dir >> 2 & 1) // Returns 1 if dir == SOUTH
#define  West(dir) (dir >> 3 & 1) // Returns 1 if dir ==  WEST

// --- Mutator Functions ---
#define LeftDir(dir)  ( (dir               >> 1) | ((dir & NORTH) << 3))
#define RightDir(dir) (((dir & NOT_WEST)   << 1) | ((dir &  WEST) >> 3))
#define BackDir(dir)  (((dir & NORTH_EAST) << 2) |  (dir          >> 2))


// ==========================
//    WALL MACRO FUNCTIONS
// ==========================

// --- Accessor Functions ---
#define   GetWallN(maze,row,col) ( (maze).walls.n[row] >> col & 1)
#define   GetWallE(maze,row,col) ( (maze).walls.e[col] >> row & 1)
#define   GetWallS(maze,row,col) ( (maze).walls.s[row] >> col & 1)
#define   GetWallW(maze,row,col) ( (maze).walls.w[col] >> row & 1)
#define GetMappedN(maze,row,col) ((maze).mapped.n[row] >> col & 1)
#define GetMappedE(maze,row,col) ((maze).mapped.e[col] >> row & 1)
#define GetMappedS(maze,row,col) ((maze).mapped.s[row] >> col & 1)
#define GetMappedW(maze,row,col) ((maze).mapped.w[col] >> row & 1)

// --- Wall Mutator Functions ---
#define SetWallN(maze,row,col,wall) ((maze).walls.n[row] = (maze).walls.n[row] & (!wall<<col^0xFFFF) | (wall<<col))
#define SetWallE(maze,row,col,wall) ((maze).walls.e[col] = (maze).walls.e[col] & (!wall<<row^0xFFFF) | (wall<<row))
#define SetWallS(maze,row,col,wall) ((maze).walls.s[row] = (maze).walls.s[row] & (!wall<<col^0xFFFF) | (wall<<col))
#define SetWallW(maze,row,col,wall) ((maze).walls.w[col] = (maze).walls.w[col] & (!wall<<row^0xFFFF) | (wall<<row))

// --- Mapped Mutator Functions ---
#define   MapWallN(maze,row,col) ((maze).mapped.n[row] |=  1 << col)
#define   MapWallE(maze,row,col) ((maze).mapped.e[col] |=  1 << row)
#define   MapWallS(maze,row,col) ((maze).mapped.s[row] |=  1 << col)
#define   MapWallW(maze,row,col) ((maze).mapped.w[col] |=  1 << row)
#define UnmapWallN(maze,row,col) ((maze).mapped.n[row] &= (1 << col ^ 0xFFFF))
#define UnmapWallE(maze,row,col) ((maze).mapped.e[col] &= (1 << row ^ 0xFFFF))
#define UnmapWallS(maze,row,col) ((maze).mapped.s[row] &= (1 << col ^ 0xFFFF))
#define UnmapWallW(maze,row,col) ((maze).mapped.w[col] &= (1 << row ^ 0xFFFF))


// ==============================
//    MAZE INTERFACE FUNCTIONS
// ==============================

// --- Initialization Functions ---
void Maze_Init(Maze *maze);

// --- Command Functions ---
Command Maze_GetCmd(Maze *maze, int myRow, int myCol, Direction myDir, int endRow, int endCol);
void    Maze_SetCmd(Maze *maze, int myRow, int myCol, Direction myDir);

// --- Wall Functions ---
void   Maze_MapWall  (Maze *maze, int myRow, int myCol, Direction dir, Wall wall);
Wall   Maze_GetWall  (Maze *maze, int myRow, int myCol, Direction wallDir);
Mapped Maze_GetMapped(Maze *maze, int myRow, int myCol, Direction wallDir);

// --- Floodfill Functions ---
uint16_t Maze_GetFlood(Maze *maze, int row, int col, Direction dir);
void Maze_Floodfill(Maze *maze, int endRow, int endCol, int startRow, int startCol, Mapped check);
void Maze_FloodfillGP(Maze *maze, int endRow, int endCol, int startRow, int startCol);

#endif

