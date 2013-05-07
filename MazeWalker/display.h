/*  File:  display.h
    By:    Tep Dobry
    login: tep
    Date:  12 Apr 08
 */

/* NOTE: this file is under construction  */


/*  This file contains the prototypes for useful funcitons in the display.o
	library  */

void init_display(int level);
/* This function initializes the display environment and clears the screen 
   and draws the template maze from the local file maze.template.  The
   parameter, level, determines the level of difficulty (see the spec).
   For Level 0 and 1 it also initializes the maze from the local file
   maze.dat.
 */

void show_mouse(int dir, int row, int col);
/* This function shows the mouse at the row and col position headed in
   the direction dir. Available at all levels.
 */

void write_message(char str[], int line);
/* This function writes the given message at one of the lines (0 to 10)
   on the bottom of the screen.  The string is truncated at 64 characters.
   Available at all levels.
 */

void clear_screen(void);
/* This function clears the screen and shuts down the display environment
   Available at all levels.
 */

void show_position(int y, int x);
/*  This functions displays the position under the X and Y on the right
    of the screen.  Available at all levels.
 */

void show_cells(int cells);
/*  This function displays the number of cells explored under CELLS on the
    right of the screen.  Available at Level 1 and higher.
 */

void show_unique_cells(int cells);
/*  This function displays the number of unique cells explored under
    UNIQUE CELLS on the right of the screen. Available at Level 2 or higher.
 */

void mouse_cam(int left, int front, int right);
/*  This functions displays the mouse view of left, front and side walls
     under MOUSE CAM on the right side of the screen. Available at all
     levels.
 */

void lvl0_get_walls(int y, int x, int *North, int *East, int *West, int *South);
/*  This function indirectly returns the walls (1 for present, 0 of absent)
    in the cell y, x from the internal maze.  Avaliable only at Level 0
    or 1.
 */

void put_walls(int y, int x, int North, int East, int West, int South);
/*  This function shows the given North, East, West and South walls
    (1 for present, 0 for absent) for the cell y, x on the screen. 
    Avaliable only at Level 1 or higher.
 */

