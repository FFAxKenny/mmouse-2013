
#include "mouse.h"

void Mouse_RunCommand(Command cmd, Maze *maze, int *myRow, int *myCol, Direction *myDir, bool map)
{
    switch(cmd)
    {
        case STOP:
            TURN_DELAY;
            Track_RL = TRACK_NONE;
            Track_F = FRONT_STOP_OFF;
            Motor_Turn180;
            *myDir = BackDir(*myDir);
            while(Motor_Busy);
            TURN_DELAY;
            break;
	case LEFT90:
            TURN_DELAY;
            Track_RL = TRACK_NONE;
            Track_F = FRONT_STOP_OFF;
            Motor_TurnL90;
            *myDir = LeftDir(*myDir);
            while(Motor_Busy);
            TURN_DELAY;
            break;
        case RIGHT90:
            TURN_DELAY;
            Track_RL = TRACK_NONE;
            Track_F = FRONT_STOP_OFF;
            Motor_TurnR90;
            *myDir = RightDir(*myDir);
            while(Motor_Busy);
            TURN_DELAY;
            break;
        case FORWARD:
            break;
    }

    // Update Position to Next Cell and Drive into Next Cell
    Move(*myRow,*myCol,*myDir);
    Motor_Forward(MAPPING_SPEED,STEPS_PER_CELL);

    // Map Left and Right Walls and Enable Tracking
    while((RightMotor_Steps+LeftMotor_Steps)/2 >= STEPS_PER_CELL-STEPS_PRE_MAP);
    if(map) Mouse_MapRL(maze, *myRow, *myCol, *myDir);
    Mouse_TrackRL(maze, *myRow, *myCol, *myDir);
    while((RightMotor_Steps+LeftMotor_Steps)/2 >= 200);
    Track_RL = TRACK_NONE;

    // Map Front Wall and Enable Tracking
    if(map) Mouse_MapF(maze, *myRow, *myCol, *myDir);
    Mouse_TrackF(maze, *myRow, *myCol, *myDir);
    while(Motor_Busy);
    Motor_Forward(MAPPING_SPEED,50);
    while(Motor_Busy);

}

bool Mouse_SpeedRun(Maze *maze, int *myRow, int *myCol, Direction *myDir, uint16_t speed)
{
    *myRow = START_ROW;
    *myCol = START_COL;
    *myDir = START_DIR;

    Sensor_WaitHand();
    __delay32(FCY);
    Track_RL = TRACK_NONE;
    Motor_Forward(MAPPING_SPEED,STEPS_PER_CELL/2);
    Move(*myRow,*myCol,*myDir);
    while(Motor_Busy);

    while(!CenterPos(*myRow,*myCol) && !button)
    {
        switch(maze->cell[*myRow][*myCol].cmd)
        {
            case LEFT90:
                Mouse_TrackRL(maze, *myRow, *myCol, *myDir);
                *myDir = LeftDir(*myDir);
                Motor_L90S(speed);
                Move(*myRow,*myCol,*myDir);
                Mouse_TrackRL(maze, *myRow, *myCol, *myDir);
                while(Motor_Busy);
                break;
            case RIGHT90:
                Mouse_TrackRL(maze, *myRow, *myCol, *myDir);
                *myDir = RightDir(*myDir);
                Motor_R90S(speed);
                Move(*myRow,*myCol,*myDir);
                Mouse_TrackRL(maze, *myRow, *myCol, *myDir);
                while(Motor_Busy);
                break;
            case FORWARD:
                Mouse_TrackRL(maze, *myRow, *myCol, *myDir);
                Motor_Forward(speed,STEPS_PER_CELL/2);
                while(Motor_Busy);
                Move(*myRow,*myCol,*myDir);
                Track_RL = TRACK_NONE;
                Motor_Forward(speed,STEPS_PER_CELL/2);
                while(Motor_Busy);
                break;
        }
    }
    // Return False if Maze was Unsolved
    if(button == true) return false;

    Motor_Forward(MAPPING_SPEED,STEPS_PER_CELL/2);
    while(Motor_Busy);

    // Return True if Maze was Solved
    return true;
}

void Mouse_MapRL(Maze *maze, int myRow, int myCol, Direction myDir)
{
    Track_Wall temp = Track_RL;
    Track_RL = TRACK_NONE;
    Maze_MapWall(maze,myRow,myCol,RightDir(myDir),Sensor_Read(SENSOR_LEFT) > R_WALL_MAP);
    Maze_MapWall(maze,myRow,myCol,LeftDir(myDir),Sensor_Read(SENSOR_RIGHT) > L_WALL_MAP);
    Track_RL = temp;
}

void Mouse_MapF(Maze *maze, int myRow, int myCol, Direction myDir)
{
    Track_Wall temp = Track_RL;
    Track_RL = TRACK_NONE;
    Maze_MapWall(maze,myRow,myCol,myDir,Sensor_Read(SENSOR_FRONT) > F_WALL_MAP);
    Track_RL = temp;
}

void Mouse_TrackRL(Maze *maze, int myRow, int myCol, Direction myDir)
{
    if     (Maze_GetWall(maze,myRow,myCol,LeftDir(myDir)))  Track_RL = TRACK_LEFT;
    else if(Maze_GetWall(maze,myRow,myCol,RightDir(myDir))) Track_RL = TRACK_RIGHT;
    else                                                    Track_RL = TRACK_NONE;
}

void Mouse_TrackF(Maze *maze, int myRow, int myCol, Direction myDir)
{
    if(Maze_GetWall(maze,myRow,myCol,myDir)) Track_F = FRONT_STOP_ON;
    else                                     Track_F = FRONT_STOP_OFF;
}
