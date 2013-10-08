#define rawMaze_height 33
#define rawMaze_width  65
#define mazeH 16
#define mazeW 16
#define rawH 33
#define rawW 65
#define RAWH 33
#define RAWW 65

void RawMaze_readRawMaze(char rawMaze[rawMaze_height][rawMaze_width]);
void RawMaze_printRawMaze(char rawMaze[rawMaze_height][rawMaze_width]);

int RawMaze_checkRawNorth(char rawmaze[rawH][rawW], int x, int y);
int RawMaze_checkRawSouth(char rawmaze[rawH][rawW], int x, int y);
int RawMaze_checkRawEast(char rawmaze[rawH][rawW], int x, int y);
int RawMaze_checkRawWest(char rawmaze[rawH][rawW], int x, int y);

int RawMaze_rawToMazeX(int rawX);
int RawMaze_rawToMazeY(int rawY);
int RawMaze_mazeToRawX(int mazeX);
int RawMaze_mazeToRawY(int mazeY);

void RawMaze_checkCell(char rawmaze[rawH][rawW], int x, int y);
