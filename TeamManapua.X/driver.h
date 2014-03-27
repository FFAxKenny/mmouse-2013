/* 
 *
 *
 * Driver.h
 *
 *
 *
 */

// standard includes
#include <stdio.h>
#include <stdlib.h>
#include "HardwareCommon.h"
#include "FloodFill.h"
#include "Cell.h"
#include "adc.h"

#define FLOOD_FILL 0
#define LEFT_WALL_HUGGER 1
#define RIGHT_WALL_HUGGER 2
#define ALGORITHM 1

// I forget what this does..something to do with clock
#pragma config ICS = PGD2
#pragma config FWDTEN = OFF             // Watchdog Timer Enable bit (Watchdog timer enabled/disabled by user software)

// Configuration Options
_FOSCSEL(FNOSC_FRC & IESO_OFF);                     // Select Oscillator
_FOSC(FCKSM_CSECMD & OSCIOFNC_ON & POSCMD_NONE);   // Some other stuff

Cell mouseMaze[16][16];
StackA nextLevel;
StackA currentLevel;


typedef struct position{
    int x;
    int y;
    int dir;
} Position;

// Declare the motors
Motor lMotor;
Motor rMotor;

Cell mouseMaze[16][16];
Position mousePos;

    /* Variables */
    double currentCellDist = 0;
    int forward_flag = 0;
    int nextMove = 0;
    int sample_flag = 0;
    int floodMoveDone = TRUE;

    int pK = 60;
    int pD = 250;
    int pKdefault = 50; 
    int pDdefault = 250;
    int savedpK;
    int savedpD;

    int algorithm;
    int destY;
    int destX;

    int floodL;
    int floodR;
    int floodF;

    int error = 0;
    int prevError = 0;
    int right = 0;
    int left = 0;
    int front = 0;
    int speedValue = 18000;
    int slowSpeedValue = 18000;
    int fastSpeedValue = 18000;

    int normalV = 20000;
    int timerBaseVal = 20000;
    int accelRate = 1;
    int decelRate = 1;
    int finalAccelV = 20000;
    int finalDecelV = 20000;

    /* Init Routines */
    void initAD(void);
    void initPLL(void);
    void initTimer1(void);
    void initTimer2(void);
    void initPins(void);
    void initMotors(void);
    void initRoutine(void);
    
    /* Analog to digital */
    void delayMicro(unsigned int delay);

    /* Sensor Functions */
    void allEmitters(int state);
    void powerEmitters(int state);
    void sampleAllSensors();

    /* Movement Routines */
    void turn90(int direction);
    void turn360(int direction);
    void moveCell(int n);
    
    /* Power Functions */
    void powerMotors(int state);
    
    /* Enabler */
    inline void enableTimer(int n);
    inline void disableTimer(int n);
    inline void waitForStart(void);
    inline void powerEmitters(int state);
    inline void powerMotors(int state);

    /* Motor Routines */
    void updateMotorStates(void);

    /* Misc */
    void executeMove(int move);
    int getMove(int a);
    int abs(int n);
    void mouseDelay(void);
    void alignToFront(void);

    /* FloodFill Moves */
    int getMoveFlood(void);
    void sampleFlood(int *floodL, int *floodF, int *floodR);
    void mapWalls(void);
    int isCenter(Position *p);
    int isStart(Position *p);
    
    /* Position Functions */
    void Position_forwardCell(Position *mousePos);
    void Position_updateDirection(Position *mousePos, int turn);
    int Position_isInCell(Position *p, int y, int x);

    /* Mouse Functions */
    void Mouse_setAlgorithm(int a);
    void Mouse_initPosition(void);
    void Mouse_setDestCell(int y, int x);
    int Mouse_isInCenterCell(void);
    int Mouse_isInStartCell(void);







