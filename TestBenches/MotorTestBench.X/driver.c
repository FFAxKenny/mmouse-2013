/* * File:   driver.c
 * Author: kenny
 *
 * Created on February 9, 2013, 11:10 PM
 */

// User Created H Files
#include "driver.h"
#include "dirdef.h"

// I forget what this does..something to do with clock
#pragma config ICS = PGD2

// Configuration Options
_FOSCSEL(FNOSC_FRC & IESO_OFF);                     // Select Oscillator
_FOSC(FCKSM_CSECMD & OSCIOFNC_ON & POSCMD_NONE);   // Some other stuff

//void funMazeInit();
// Declare the motors
Motor lMotor;
Motor rMotor;

Cell mouseMaze[16][16];
StackA nextLevel;
StackA currentLevel;

double currentCellDist = 0;
int forward_flag = 0;
int nextMove = 0;

Position mousePos;
int localToGlobalDir(int localDir, int currDir);
int globalToLocalDir(int globalDir, int currDir);
int getMoveFlood(void);
int floodMoveDone= 1;
int floodR;
int floodL;
int floodF;

void sampleSensors(int *L, int *F, int *R) {
    *L += sampleSensor(L90_SENSOR);
    *R += sampleSensor(R90_SENSOR);
    *F += sampleSensor(F1_SENSOR);
    *L += sampleSensor(L90_SENSOR);
    *R += sampleSensor(R90_SENSOR);
    *F += sampleSensor(F1_SENSOR);
    *L += sampleSensor(L90_SENSOR);
    *R += sampleSensor(R90_SENSOR);
    *F += sampleSensor(F1_SENSOR);
    *L = (*L)/3;
    *R = (*R)/3;
    *F = (*F)/3;
}



int main(void) {
    double k;
    
    initRoutine();

    LATBbits.LATB13 = 1;                            // Disable Motors
    mousePos.x = 0;
    mousePos.y = 0;
    mousePos.dir = NORTH;

    FloodFill_initMaze();
    //funMazeInit();

    waitForStart();                                 // Wait for the start input
    for(k = 0; k< 150000; k++);                     // Delay the start

    ADC1BUF0 = 0;                                   // Clear the buffer sampleAllSensors();
    powerMotors(ON);
    enableTimer(1);
    enableTimer(2);

    /*
    Maze_putWall(mouseMaze, mousePos.y, mousePos.x, NORTH);
    Maze_putWall(mouseMaze, 0 , 1 , NORTH);
    Maze_putWall(mouseMaze, 0 , 2 , NORTH);
    Maze_putWall(mouseMaze, 0 , 3 , NORTH);
    nextMove = globalToLocalDir(Maze_smallestNeighborDir(&mouseMaze[0][0]), mousePos.dir);
    */

    sampleSensors(&floodL, &floodF, &floodR);
    nextMove=getMoveFlood();

    while(!isCenter(&mousePos)){
        executeMove(nextMove);
    }
    while(1){
        disableTimer(1); 
        disableTimer(2);
    }



}
/* 
 *
 * Given a move, executes a move. This function is the main driving function behind the mouse
 *
 */
void executeMove(int move)
{
    switch(move)
    {
        case LEFT:
            forward_flag = 0;
            turn90(LEFT);
            moveCell(1);
            break;
        case RIGHT:
            forward_flag = 0;
            turn90(RIGHT);
            moveCell(1);
            break;
        case BACKWARD:
            forward_flag = 0;
            turn360(1);
            moveCell(1);
            break;
        case FORWARD:
            forward_flag = 1;
            moveCell(1);
            break;
        default:
            moveCell(1);
            break;
    }
}

/*
 *
 * Converts a global maze direction to a local maze direction.
 * This function is used primarily to help maze tracking
 *
 */

int globalToLocalDir(int globalDir, int currDir){
    switch(currDir) {
        case NORTH:
            return globalDir;
            break;
        case EAST:
            if(globalDir == NORTH) return LEFT;
            else if(globalDir == EAST) return FORWARD;
            else if(globalDir == SOUTH) return RIGHT;
            else if(globalDir == WEST) return BACKWARD;
            break;
        case SOUTH:
            if(globalDir == NORTH) return BACKWARD;
            else if(globalDir == EAST) return LEFT;
            else if(globalDir == SOUTH) return FORWARD;
            else if(globalDir == WEST) return RIGHT;
            break;
        case WEST:
            if(globalDir == NORTH) return RIGHT;
            else if(globalDir == EAST) return BACKWARD;
            else if(globalDir == SOUTH) return LEFT;
            else if(globalDir == WEST) return FORWARD;
            break;
        default:
            return NORTH;
    }
}

/*
 *
 * Converts a local maze direction to a global maze direction.
 * This function is used primarily to help maze tracking
 *
 */
int localToGlobalDir(int localDir, int currDir){
    switch(currDir) {
        case NORTH:
            if(localDir == LEFT) return WEST;
            else if(localDir == FRONT) return NORTH;
            else if(localDir == RIGHT) return EAST;
            else return NORTH;
        case EAST:
            if(localDir == LEFT) return NORTH;
            else if(localDir == FRONT) return EAST;
            else if(localDir == RIGHT) return SOUTH;
            else return NORTH;
        case SOUTH:
            if(localDir == LEFT) return EAST;
            else if(localDir == FRONT) return SOUTH;
            else if(localDir == RIGHT) return WEST;
            else return NORTH;
        case WEST:
            if(localDir == LEFT) return SOUTH;
            else if(localDir == FRONT) return WEST;
            else if(localDir == RIGHT) return NORTH;
            else return NORTH;
        default: 
            return NORTH;
    }
}

/*
 *
 * Calculates a move using floodFill
 * 
 */
int getMoveFlood(void){
    floodMoveDone = FALSE;
    if(floodL > LEFT_THRESHOLD) {
        Maze_putWall(mouseMaze, mousePos.y, mousePos.x, localToGlobalDir(LEFT, mousePos.dir));
        //turn90(LEFT);
        //turn90(RIGHT);
    }
    if(floodR > RIGHT_THRESHOLD) {
        Maze_putWall(mouseMaze, mousePos.y, mousePos.x, localToGlobalDir(RIGHT, mousePos.dir));
        //turn90(RIGHT);
        //turn90(LEFT);
    }
    if(floodF > FRONT_THRESHOLD) {
        Maze_putWall(mouseMaze, mousePos.y, mousePos.x, localToGlobalDir(FRONT, mousePos.dir));
        //turn360(RIGHT);
        //turn360(LEFT);

    }

    FloodFill_floodMaze(); 
    floodMoveDone = TRUE;
    return globalToLocalDir(Maze_smallestNeighborDir(&mouseMaze[mousePos.y][mousePos.x]), mousePos.dir);

}

/*
 *
 * Calculates a move using wall hugging.
 *
 */

int getMove(void){
    sampleAllSensors();
    if(left < LEFT_THRESHOLD) {
        return LEFT;
    }
    // if left is covered
    else {
        // if front is open
        if(front < FRONT_THRESHOLD) {
            return FORWARD;
        }
        // if front is closed
        else {
            // if right is open
            if(right < RIGHT_THRESHOLD)
                return RIGHT;
            // if right is closed
            else
                return BACKWARD;
        }

    }
}
    

void sampleAllSensors(){
    int tempLeft = 0;
    int tempRight = 0;
    int tempFront = 0;
    int sampleNumber = 3;
    int i; 

    for( i = 0; i < sampleNumber ; i++) {
        tempLeft = tempLeft + sampleSensor(L90_SENSOR);
        tempRight = tempRight + sampleSensor(R90_SENSOR); 
        tempFront = tempFront + sampleSensor(F1_SENSOR);
    }
    left = tempLeft/sampleNumber;
    right = tempRight/sampleNumber;
    front = tempFront/sampleNumber;
}

void mouseDelay(void) {
    __delay32(5000000);
    __delay32(5000000);
    __delay32(5000000);
}

void moveCell(int n)
{
    double temp;
    double temp2;
    int error = 0;
    int tempError = 0;
    int accel = 25000;
    forward_flag = 1;
    int sample_flag = FALSE;

    temp2 = lMotor.count;
    currentCellDist = lMotor.count - temp2;
    while( currentCellDist < CELL_DISTANCE) {
        currentCellDist = lMotor.count - temp2;
        temp = lMotor.count;

        if(currentCellDist > (800) && sample_flag == FALSE) {
            sample_flag = TRUE;
        }

        sampleAllSensors();
        if(right > 35) error = right - 150;
        else if( left > 35 ) error = 150 - left;
        else error = 0;

        if(error < 0) tempError = -error;
        else tempError = error;
        if(tempError < 8) error = 0;

        while( (lMotor.count - temp) < 2) {
            PR1 = accel - error*pK -  (error-prevError)*pD;  // Left Motor
            PR2 = accel + error*pK + (error-prevError)*pD;  // Right Motor prevError = error;
        }

    }

    Position_forwardCell(&mousePos);
    sampleSensors(&floodL, &floodF, &floodR);
    nextMove=getMoveFlood();

    temp = 0;
    temp2 = 0;
    lMotor.count = 0;
    rMotor.count = 0;
    sample_flag = FALSE;
    currentCellDist = 0;

}
int abs (int n) {
        const int ret[2] = { n, -n };
            return ret [n<0];
}
void turn90(int direction) {
    disableTimer(1);
    disableTimer(2);
    __delay32(5000000);
    __delay32(5000000);
    __delay32(5000000);
    PR1 = 30000;
    PR2 = 30000;
    if(direction == RIGHT){
        Position_updateDirection(&mousePos, RIGHT);
        rMotor.dir = 0;
    }
    else if(direction == LEFT){
        Position_updateDirection(&mousePos, LEFT);
        lMotor.dir = 0;
    }
    long temp = lMotor.count;
    while( (lMotor.count - temp) < DISTANCE_90)
    {
        enableTimer(1);
        enableTimer(2);
    }
    disableTimer(1);
    disableTimer(2);
    lMotor.dir = 1;
    rMotor.dir = 1;
    PR1= 16000;
    PR2= 16000;
    __delay32(5000000);
    __delay32(5000000);
    __delay32(5000000);
    sampleAllSensors();
    enableTimer(1);
    enableTimer(2);
}

void turn360(int direction) {
    disableTimer(1);
    disableTimer(2);
    __delay32(5000000);
    __delay32(5000000);
    __delay32(5000000);
    PR1 = 30000;
    PR2 = 30000;
    lMotor.dir = 0;
    long temp = lMotor.count;
    while( (lMotor.count - temp) < DISTANCE_360)
    {
        enableTimer(1);
        enableTimer(2);
    }
    Position_updateDirection(&mousePos, BACKWARD);
    disableTimer(1);
    disableTimer(2);
    lMotor.dir = 1;
    __delay32(5000000);
    __delay32(5000000);
    __delay32(5000000);
    PR1= 16000;
    PR2= 16000;
    sampleAllSensors();
    enableTimer(1);
    enableTimer(2);
}

void enableTimer(int n) {
    switch(n) {
        case 1: 
            T1CONbits.TON = 1;                              // Enable Timer
            break;
        case 2:
            T2CONbits.TON = 1;                              // Enable Timer
            break;
        case 3:
            break;
        case 4:
            break;
        default:
            break;
    }
}

void disableTimer(int n) {
    switch(n) {
        case 1: 
            T1CONbits.TON = 0;                              // Enable Timer
            break;
        case 2:
            T2CONbits.TON = 0;                              // Enable Timer
            break;
        case 3:
            break;
        case 4:
            break;
        default:
            break;
    }
}

/*********************************************************************
 *      Motor Interrupt Service Routines
 *********************************************************************/ 
void __attribute__((__interrupt__, __auto_psv__)) _T1Interrupt(void){
        _T1IF = 0;      // Reset the timer flag
        if( rMotor.enable ) 
            Motor_step(&rMotor);
        __PIN_MotorRStep = rMotor.step;   // Update right motor state
        __PIN_MotorRDir = rMotor.dir;
}
void __attribute__((__interrupt__, __auto_psv__)) _T2Interrupt(void){
        _T2IF = 0;      // Reset the timer flag
        if( lMotor.enable)
            Motor_step(&lMotor);
        __PIN_MotorLStep = lMotor.step;   // Update left motor state
        __PIN_MotorLDir = lMotor.dir;
}

void updateMotorStates(void)
{
    __PIN_MotorRStep = rMotor.step;   // Update right motor state
    __PIN_MotorLStep = lMotor.step;   // Update left motor state
    __PIN_MotorRDir = rMotor.dir;
    __PIN_MotorLDir = lMotor.dir;
}

/*********************************************************************
 *      Routine Functions
 *********************************************************************/ 
inline void waitForStart(void){
    while( sampleSensor(R90_SENSOR) < 400 );         // Wait for start input
}
inline void powerEmitters(int state){

}
inline void powerMotors(int state){
    LATBbits.LATB13 = 0;                            // Enable Motors
}

/*********************************************************************
 *      Analog to Digital Functions
 *********************************************************************/ 
int sampleSensor(int sensor)
{
    AD1CON1bits.ADON = 0;
    switch(sensor)
    {
        case L90_SENSOR:
            AD1CHS0 = 0x0003;                 
            __PIN_EmitL90 = 1;
            AD1CON1bits.ADON = 1;
            sampleAD();
            __PIN_EmitL90 = 0;
            break;
        case F1_SENSOR:
            AD1CHS0 = 0x0002;               
            __PIN_EmitF1 = 1;
            AD1CON1bits.ADON = 1;
            sampleAD();
            __PIN_EmitF1 = 0;
            break;

        case R90_SENSOR:
            AD1CHS0 = 0x0004;               
            __PIN_EmitR90 = 1;
            AD1CON1bits.ADON = 1;
            sampleAD();
            __PIN_EmitR90 = 0;
            break;
        case F2_SENSOR:
            AD1CHS0 = 0x0005;               
            __PIN_EmitF2 = 1;
            AD1CON1bits.ADON = 1;
            sampleAD();
            __PIN_EmitF2 = 0;
            break;

        case R45_SENSOR:
            AD1CHS0 = 0x0005;               
            __PIN_EmitR45 = 1;
            AD1CON1bits.ADON = 1;
            sampleAD();
            __PIN_EmitR45 = 0;
            break;
        case L45_SENSOR:
            AD1CHS0 = 0x0004;               
            __PIN_EmitL45 = 1;
            AD1CON1bits.ADON = 1;
            sampleAD();
            __PIN_EmitL45 = 0;
            break;
    }
    return ADC1BUF0;
}

void sampleAD(void)
{
    // Actually sample
    AD1CON1bits.ADON = 1;
    delayMicro(20);
    AD1CON1bits.SAMP = 0;
    while (!AD1CON1bits.DONE);
    AD1CON1bits.DONE = 0;
}


/*********************************************************************
 *     Initialization Functions 
 *********************************************************************/ 
void initMotors(void) {
    Motor_init(&lMotor);
    Motor_init(&rMotor);
}
void initPins(void) {
    LATBbits.LATB9=1;
    LATBbits.LATB6=1;
    LATBbits.LATB5=1;
    LATAbits.LATA4=1;
    
    ANSELA = 0;//configures pin B4 as digital
    ANSELB = 0;//configures pin B4 as digital

    TRISA = 0b00000000;      // Configure B Ports as output
    TRISB = 0b00000000;      // Configure B Ports as output

    TRISBbits.TRISB15 = 1;
    TRISBbits.TRISB3 = 1;
    TRISBbits.TRISB2 = 1;

    // Configure Phototransistor Pins
    TRISAbits.TRISA0  = 1;   
    TRISAbits.TRISA1  = 1;   
    TRISBbits.TRISB0  = 1;   
    TRISBbits.TRISB1  = 1;  
    TRISBbits.TRISB2  = 1;  
    TRISBbits.TRISB3  = 1;  
    
    LATBbits.LATB5 = 1;
    LATBbits.LATB6 = 1;
    LATBbits.LATB14 = 1;


    // Make sure that the emitters are off
    allEmitters(OFF);
    
}

void allEmitters(int state) {
    LATAbits.LATA2 = state;             // L90
    LATAbits.LATA3 = state;             // FL
    LATBbits.LATB4 = state;             // FR
    LATAbits.LATA4 = state;             // R90
    LATBbits.LATB4 = state;
    LATBbits.LATB10 = state;
}

void initAD(void) {
    ANSELAbits.ANSA0 = 1;         // Set pin A0 as analog 
    ANSELBbits.ANSB0 = 1;         // Set pin A0 as analog 
    ANSELBbits.ANSB1 = 1;         // Set pin A0 as analog 
    ANSELBbits.ANSB2 = 1;         // Set pin A0 as analog 
    ANSELBbits.ANSB3 = 1;         // Set pin A0 as analog 

    AD1CON1 = 0x0004;
    AD1CON2 = 0x0000;
    AD1CON3 = 0x000F;
    AD1CON4 = 0x0000;
    AD1CHS0 = 0x0005;
    AD1CHS123 = 0x0000;
    AD1CSSH = 0x0000;
    AD1CSSL = 0x0000;
    //AD1CON1bits.ADON = 1;

    delayMicro(20);
}


void initPLL(void) {
    /********************************
     *      PLL Configuration
     ********************************/
    // Configure PLL prescaler, PLL postscaler, PLL divisor
    PLLFBD=63;              // M=65
    CLKDIVbits.PLLPOST=0;   // N2=2
    CLKDIVbits.PLLPRE=1;    // N1=3

    // Initiate Clock Switch to FRC oscillator with PLL (NOSC=0b001)
    __builtin_write_OSCCONH(0x01);
    __builtin_write_OSCCONL(OSCCON | 0x01);

    // Wait for Clock switch to occur
    while (OSCCONbits.COSC!= 0b001);
    // Wait for PLL to lock
    while (OSCCONbits.LOCK!= 1);
}

void initTimer1(void) {
    /********************************
     *      Timer1 Configuration
     ********************************/
    T1CON = 0;               // Reset T1 Configuration
    T1CONbits.TCKPS = 0;     // Set the ratio to the highest
    PR1 = 16000;             // Set the timer

    _T1IP = 1;
    _T1IF = 0;
    _T1IE = 1;
    T1CONbits.TON = 0;       // Enable Timer

}

void initTimer2(void) {
    /********************************
     *      Timer2 Configuration
     ********************************/
    T2CON = 0;               // Reset T1 Configuration
    T2CONbits.TCKPS = 0;     // Set the ratio to the highest
    PR2 = 16000;             // Set the timer

    _T2IP = 1;
    _T2IF = 0;
    _T2IE = 1;
    T2CONbits.TON = 0;       // Enable Timer
}

/*********************************************************************
 *     Initialization Functions 
 *********************************************************************/ 
void delayMicro(unsigned int delay) {
    int i;
    for (i = 0; i < delay; i++) {
    __asm__ volatile ("repeat #39");
    __asm__ volatile ("nop");
    }

}
void initRoutine(void)
{
    // Init stuff
    initMotors();
    initPins();
    initTimer1();
    initTimer2();
    initPLL();
    initAD();
}

void Position_forwardCell(Position *mousePos) {
    switch(mousePos->dir){
        case NORTH:
            mousePos->y++;
            break;
        case EAST:
            mousePos->x++;
            break;
        case WEST:
            mousePos->x--;
            break;
        case SOUTH:
            mousePos->y--;
            break;
        default:
            break;
    }

}
void Position_updateDirection(Position *mousePos, int turn) {
    switch(turn){
        case RIGHT:
            if(mousePos->dir == NORTH) mousePos->dir = EAST;
            else if(mousePos->dir == EAST) mousePos->dir = SOUTH;
            else if(mousePos->dir == WEST) mousePos->dir = NORTH;
            else if(mousePos->dir == SOUTH) mousePos->dir = WEST;
            break;
        case LEFT:
            if(mousePos->dir == NORTH) mousePos->dir = WEST;
            else if(mousePos->dir == EAST) mousePos->dir = NORTH;
            else if(mousePos->dir == WEST) mousePos->dir = SOUTH;
            else if(mousePos->dir == SOUTH) mousePos->dir = EAST;
            break;
        case BACKWARD:
            if(mousePos->dir == NORTH) mousePos->dir = SOUTH;
            else if(mousePos->dir == WEST) mousePos->dir = EAST;
            else if(mousePos->dir == EAST) mousePos->dir = WEST;
            else mousePos->dir = NORTH;
            break;
        default:    
            break;
    }

}

int isCenter(Position *p) {
    if(p->x==8 && p->y==7) return TRUE;
    else if(p->x==7 && p->y==8) return TRUE;
    else if(p->x==8 && p->y==8) return TRUE;
    else if(p->x==7 && p->y==7) return TRUE;
    else return FALSE;
}


void funMazeInit(){
    int i = 0;
    int j = 0;
    int level = 0;
    Cell* tempCell;

    StackInit(&currentLevel);
    StackInit(&nextLevel);

    // init the maze values
    for(i = 15; i >= 0 ; i--) {
        for(j = 0; j < 16; j++) {
            mouseMaze[i][j].floodValue = 255;
            mouseMaze[i][j].x = j;
            mouseMaze[i][j].y = i;
        }
    }

    // Map the initial walls
    for( i = 0; i < 16 ; i++)
        mouseMaze[i][0].west = TRUE;
    for( i = 0; i < 16 ; i++)
        mouseMaze[i][15].east = TRUE;
    for( i = 0; i < 16 ; i++)
        mouseMaze[0][i].south = TRUE;
    for( i = 0; i < 16 ; i++)
        mouseMaze[15][i].north = TRUE;

    tempCell = &mouseMaze[7][7];
    StackPush(&currentLevel, &mouseMaze[7][7]);

    while(1){
        while(!StackIsEmpty(&currentLevel)) {
            tempCell=StackPop(&currentLevel);
            //printf("Popping %d,%d \n", tempCell->x, tempCell->y);
            if(tempCell->floodValue == 255) {
                tempCell->floodValue = level;
                FloodFill_pushNeighbors(tempCell);
            }
        }
        if(!StackIsEmpty(&nextLevel)) {
            level++;
            //printf("Level %d\n", level);
            StackCopy(&nextLevel, &currentLevel);   // Copy nextLevel into Current lev
            //StackDestroy(&nextLevel);
            StackInit(&nextLevel);
        }
        else {
            break;  // if empty, done
        }
    }
}

