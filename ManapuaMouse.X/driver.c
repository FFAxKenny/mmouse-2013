/* * File:   driver.c
 * Author: kenny
 *
 * Created on February 9, 2013, 11:10 PM
 */


// User Created H Files
#include "motor.h"
#include "pinconfig.h"
#include "driver.h"
#include "tfdef.h"
#include "dirdef.h"
#include "adc.h"
#include "config.h"
#include "FloodFill.h"

int turnPR = 40000;
int turnResetValue = 22000;

int main(void) {
    double k;

    initRoutine();

    Mouse_setAlgorithm(FLOOD_FILL);
    Mouse_initPosition();
    Mouse_setDestCell(7,7);
    FloodFill_initMaze(destY, destX);

    powerMotors(OFF);

    waitForStart();                                 // Wait for the start input
    for(k = 0; k< 150000; k++);                     // Delay the start

    powerMotors(ON);
    ADC1BUF0 = 0;            // Clear the buffer sampleAllSensors();

    nextMove=getMove(algorithm);
    enableTimer(1);
    enableTimer(2);
    while(!Mouse_isInCenterCell()){
        executeMove(nextMove);
    }
    disableTimer(1);
    disableTimer(2);
    algorithm = FLOOD_FILL;
    destX = 0;
    destY = 0;
    mouseDelay();
    enableTimer(1);
    enableTimer(2);

    while(!isStart(&mousePos)){
        executeMove(nextMove);
    }

    disableTimer(1);
    disableTimer(2);
    algorithm = FLOOD_FILL;
    destX = 7;
    destY = 7;
    mouseDelay();
    enableTimer(1);
    enableTimer(2);

    accelRate = 10;
    finalAccelV = 16000;
    decelRate = 50;
    finalDecelV = 20000;
    int savedAccelRate;
    int savedFinalAccelV;

    int timerBaseVal = 18000;

    while(1){
        turnPR = 22000;
        while(!isCenter(&mousePos)){
            executeMove(nextMove);
        }

        // save the last acceleration values
        savedAccelRate = accelRate;
        savedFinalAccelV = finalAccelV;
        savedpK = pK;
        savedpD = pD;

        disableTimer(1);
        disableTimer(2);
        algorithm = FLOOD_FILL;
        destX = 0;
        destY = 0;
        accelRate = 0;
        pK = pKdefault;
        pD = pDdefault;
        timerBaseVal = normalV;
        mouseDelay();
        enableTimer(1);
        enableTimer(2);

        while(!isStart(&mousePos)){
            executeMove(nextMove);
        }

        accelRate = savedAccelRate;
        finalAccelV = savedFinalAccelV;
        pK = savedpK;
        pD = savedpD;

        disableTimer(1);
        disableTimer(2);
        algorithm = FLOOD_FILL;
        destX = 7;
        destY = 7;
        mouseDelay();
        enableTimer(1);
        enableTimer(2);
    
        //accelRate = 2;
        accelRate = accelRate + 1;
        finalAccelV = finalAccelV - 2000;
        pK = pK + 1;
        pD = pD + 10;
    }

    while(!isStart(&mousePos)){
        executeMove(nextMove);
    }

    disableTimer(1);
    disableTimer(2);
    while(1);
}

void executeMove(int move) {
    switch(move) {
        case LEFT:
            forward_flag = 0;
            turn90(LEFT);
            timerBaseVal = turnResetValue;
            moveCell(1);
            break;
        case RIGHT:
            forward_flag = 0;
            turn90(RIGHT);
            timerBaseVal = turnResetValue;
            moveCell(1);
            break;
        case BACKWARD:
            forward_flag = 0;
            turn360(1);
            timerBaseVal = turnResetValue;
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

int getMove(int a){
    sampleAllSensors();
    switch(a){
        case LEFT_WALL_HUGGER:
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
            break;
        case RIGHT_WALL_HUGGER:
            if(right < RIGHT_THRESHOLD) {
                return RIGHT;
            }
            else {
                if(front < FRONT_THRESHOLD) {
                    return FORWARD;
                }
                else {
                    if(left < LEFT_THRESHOLD)
                        return LEFT;
                    else
                        return BACKWARD;
                }
            }
            break;
        case FLOOD_FILL:
            return getMoveFlood();
            break;
        default: 
            return FORWARD;
            break;
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


void alignToFront(void)
{
    disableTimer(1);
    disableTimer(2);
    sampleAllSensors();

    if(front >= 225) {
        while(front >= 225){
            lMotor.dir = 0;
            rMotor.dir = 0;
            enableTimer(1);
            enableTimer(2);
        }
    }
    else {
        while(front < 225){
            enableTimer(1);
            enableTimer(2);
        }
    }
    disableTimer(1);
    disableTimer(2);
}

/* Change the PR values by given rate*/
void accelerate(int rate, int finalValue) {
    int accelerateLeft;
    //accelerateLeft = finalValue - timerBaseVal;
    //rate = (1 + accelerateLeft/1000)*(1 + accelerateLeft/1000);
    if( timerBaseVal > finalValue )
        timerBaseVal = timerBaseVal - rate;
}

/* Change the PR values by given rate*/
void decelerate(int rate, int finalValue){
    if( timerBaseVal < finalValue )
        timerBaseVal = timerBaseVal + rate;
}



void moveCell(int n)
{
    double temp;
    double temp2;
    int error = 0;
    int tempError = 0;
    int accel = speedValue;
    forward_flag = 1;
    enableTimer(1);
    enableTimer(2);

    temp2 = lMotor.count;
    currentCellDist = lMotor.count - temp2;

    while( currentCellDist < CELL_DISTANCE && front < 300) {
            currentCellDist = lMotor.count - temp2;
            temp = lMotor.count;
            sampleAllSensors();


            if(currentCellDist > (850) &&
                    sample_flag == FALSE) {
                Position_forwardCell(&mousePos);
                nextMove = getMove(algorithm);
                sample_flag = TRUE;
            }

            if( nextMove == RIGHT || nextMove == LEFT || nextMove == BACKWARD && sample_flag ==TRUE)
                decelerate(decelRate, finalDecelV);
            else
                accelerate(accelRate, finalAccelV);

            if(right > 25)
                error = right - 55;
            else if( left > 20)
                error = 55 - left;
            else
                error = 0;

            if(error < 0)
                tempError = -error;
            else
                tempError = error;
            if(tempError < 5)
                error = 0;

            while( (lMotor.count - temp) < 2) {
                    PR1 = timerBaseVal - error*pK -  (error-prevError)*pD;  // Left Motor
                    PR2 = timerBaseVal + error*pK + (error-prevError)*pD;  // Right Motor
                    prevError = error;
            }
    }

        if( nextMove == RIGHT || nextMove == LEFT || nextMove == BACKWARD)
            timerBaseVal = 18000;
        /*
         *
        Position_forwardCell(&mousePos);
        nextMove = getMove(algorithm);
        */
        temp = 0;
        temp2 = 0;
        lMotor.count = 0;
        rMotor.count = 0;
        sample_flag = 0;
        currentCellDist = 0;
        /*
        disableTimer(1);
        disableTimer(2);
        mouseDelay();
        enableTimer(1);
        enableTimer(2);
        */

}
int abs (int n) {
        const int ret[2] = { n, -n };
            return ret [n<0];
}
void turn90(int direction) {
    disableTimer(1);
    disableTimer(2);
    __delay32(500000);
    PR1 = turnPR;
    PR2 = turnPR;
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
    __delay32(500000);
    sampleAllSensors();
}

void turn360(int direction) {
    disableTimer(1);
    disableTimer(2);
    __delay32(500000);
    PR1 = turnPR;
    PR2 = turnPR;
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
    __delay32(500000);
    sampleAllSensors();
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
            T3CONbits.TON = 1;       // Enable Timer
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
            T3CONbits.TON = 0;       // Enable Timer
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
// Timer 3 Service Routine
void __attribute__((__interrupt__, __auto_psv__)) _T3Interrupt(void){
        _T3IF = 0;      // Reset the timer flag
}

void updateMotorStates(void) {
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
    if(state == ON)
        state = 0;
    else 
        state = 1;
    LATBbits.LATB13 = state;                            // Enable Motors
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

void initTimer3(void) {
    /********************************
     *      Timer3 Configuration
     ********************************/
    T3CON = 0;               // Reset T1 Configuration
    T3CONbits.TCKPS = 0;     // Set the ratio to the highest
    PR3 = 10000;             // Set the timer

    _T3IP = 1;
    _T3IF = 0;
    _T3IE = 1;
    //T3CONbits.TON = 0;       // Enable Timer

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

/*
 * 
 * FloodFill Helper Functions 
 *
 */

void sampleFlood(int *floodL, int *floodF, int *floodR){
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
    *floodL = tempLeft/sampleNumber;
    *floodR = tempRight/sampleNumber;
    *floodF = tempFront/sampleNumber;
}

void mapWalls(void){
    sampleFlood(&floodL, &floodF, &floodR);
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
}

int getMoveFlood(void){
    floodMoveDone = FALSE;
    mapWalls();
    FloodFill_floodMaze(destY, destX); 
    floodMoveDone = TRUE;
    return globalToLocalDir(Maze_smallestNeighborDir(&mouseMaze[mousePos.y][mousePos.x]), mousePos.dir);
}

int isCenter(Position *p) {
    if(p->x==8 && p->y==7) return TRUE;
    else if(p->x==7 && p->y==8) return TRUE;
    else if(p->x==8 && p->y==8) return TRUE;
    else if(p->x==7 && p->y==7) return TRUE;
    else return FALSE;
}

int isStart(Position *p) {
    if(p->x==0 && p->y==0) return TRUE;
    else return FALSE;
}

/*
 * 
 * Position Functions
 *
 */

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

int Position_isInCell(Position *p, int y, int x) {
    if( p->x == x && p->y == y) return TRUE;
    else return FALSE;
}

void Mouse_setAlgorithm(int a) {
    algorithm = a;
}

void Mouse_initPosition(void){
    mousePos.x = 0;
    mousePos.y = 0;
    mousePos.dir = NORTH;
}
void Mouse_setDestCell(int y, int x){
    destY = y;
    destX = x;
}

int Mouse_isInCenterCell(void) {
    return Position_isInCell(&mousePos,7,7);
}
int Mouse_isInStartCell(void) {
    return Position_isInCell(&mousePos,0,0);
}

