#ifndef CELL_H
#define CELL_H
typedef struct cell{
    int x;
    int y;
    int floodValue;
    int north;
    int west;
    int south;
    int east;
} Cell;
#endif
