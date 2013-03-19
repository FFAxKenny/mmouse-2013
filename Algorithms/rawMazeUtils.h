#define rawMaze_height 33
#define rawMaze_width  65
#define mazeH 16
#define mazeW 16
#define rawH 33
#define rawW 65
#define RAWH 33
#define RAWW 65

void readRawMaze(char rawMaze[rawMaze_height][rawMaze_width]);
void printRawMaze(char rawMaze[rawMaze_height][rawMaze_width]);

int checkRawNorth(char rawmaze[rawH][rawW], int x, int y);
int checkRawSouth(char rawmaze[rawH][rawW], int x, int y);
int checkRawEast(char rawmaze[rawH][rawW], int x, int y);
int checkRawWest(char rawmaze[rawH][rawW], int x, int y);

int rawToMazeX(int rawX);
int rawToMazeY(int rawY);
int mazeToRawX(int mazeX);
int mazeToRawY(int mazeY);

void checkCell(char rawmaze[rawH][rawW], int x, int y);
