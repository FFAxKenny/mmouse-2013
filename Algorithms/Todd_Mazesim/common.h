
#ifndef COMMON_H
#define COMMON_H

#define DISPLAY

#ifdef DISPLAY
#include <stdio.h>
#include <time.h>
#endif

#include <stdlib.h>

/*----------------------------------------*
 *    MACROS TO CHANGE MAZE PARAMETERS    *
 *----------------------------------------*/
#define MAX_FLOOD	255
#define MAX_ROW		16
#define MAX_COL		16
#define START_ROW	(MAX_ROW-1)
#define START_COL	0
#define START_DIR	NORTH
#define START_CELL	START_ROW,START_COL
#define CENTER_CELL	-1,-1

/*-------------------------*
 *    BOOLEON DATA TYPE    *
 *-------------------------*/
typedef enum
{
	false  = 0,
	true   = 1,
	unknown = -1
}
Bool;

/*---------------------------*
 *    DIRECTION DATA TYPE    *
 *---------------------------*/
typedef enum
{
	HERE  = 0b0000,

	NORTH = 0b0001,
	EAST  = 0b0010,
	SOUTH = 0b0100,
	WEST  = 0b1000,

	NORTH_EAST = NORTH | EAST,
	NORTH_WEST = NORTH | WEST,
	SOUTH_EAST = SOUTH | EAST,
	SOUTH_WEST = SOUTH | WEST,

	NOT_NORTH = 0b1110,
	NOT_EAST  = 0b1101,
	NOT_SOUTH = 0b1011,
	NOT_WEST  = 0b0111,
}
Direction;

/*--------------------------*
 *    POSITION DATA TYPE    *
 *--------------------------*/
typedef struct
{
	char row;
	char col;
	Direction dir;
}
Position;

/*----------------------*
 *    WALL DATA TYPE    *
 *----------------------*/
typedef enum
{
	WALL    = 1,
	NOWALL  = 0
}
Wall;

/*-------------------------*
 *    MAPPING DATA TYPE    *
 *-------------------------*/
typedef enum
{
	MAPPED   = 1,
	UNMAPPED = 0
}
Mapped;

/*-------------------------*
 *    COMMAND DATA TYPE    *
 *-------------------------*/
typedef enum
{
	// Bits:      000 |     001 |     010 |     011 |  100 |  101 |   110 |   111 |
	// Bit 2: Forward | Forward | Forward | Forward | Turn | Turn | Turn  | Turn  |
	// Bit 1: Same    | Same    | Change  | Change  | Left | Left | Right | Right |
	// Bit 0: XXXXXXX | Stop    | Faster  | Slower  | 45   | 90   | 45    | 90    |

	FORWARD = 0b000,
	STOP    = 0b001,
	FASTER  = 0b010,
	SLOWER  = 0b011,
	LEFT45  = 0b100,
	LEFT90  = 0b101,
	RIGHT45 = 0b110,
	RIGHT90 = 0b111
}
Command;

/*----------------------*
 *    CELL DATA TYPE    *
 *----------------------*/
typedef union
{
	struct
	{
		unsigned short flood:        8; // Keeps track of the Flood Fill value
		unsigned short north:        1; // Keeps track of the north wall
		unsigned short west:         1; // Keeps track of the west wall
		unsigned short north_mapped: 1; // Checks if the north wall has been mapped
		unsigned short west_mapped:  1; // Checks if the west wall has been mapped
		unsigned short visited:      1; // Checks if the cell has been visited
		unsigned short command:      3;	// Checks what instruction should be executed
	};
} Cell;

/*----------------------*
 *    MAZE DATA TYPE    *
 *----------------------*/
typedef Cell Maze[MAX_ROW][MAX_COL];

#endif

