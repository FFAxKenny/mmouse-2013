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
#include <xc.h>
#include <dsp.h>
#include <p33Exxxx.h>

#include "FloodFill.h"
#include "motor.h"
#include "pinconfig.h"
#include "tfdef.h"
#include "dirdef.h"
#include "adc.h"
#include "config.h"
#include "Cell.h"
#include "stackA.h"
#include "stack.h"


    typedef struct position{
        int x;
        int y;
        int dir;
    } Position;

    /* Variables */
    int error = 0;
    int prevError = 0;
    int right = 0;
    int left = 0;
    int front = 0;
    int speedValue = 16000;


    /* Init Routines */
    void initAD(void);
    void initPLL(void);
    void initTimer1(void);
    void initTimer2(void);
    void initPins(void);
    void initMotors(void);
    void initRoutine(void);
    
    /* Analog to digital */
    void sampleAD(void);
    void delayMicro(unsigned int delay);

    /* Sensor Functions */
    int sampleSensor(int sensor);
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
    int getMove(void);
    int abs(int n);
    void mouseDelay(void);

    /* Position Tracking */
    extern void Position_forwardCell(Position *mousePos);
    extern void Position_updateDirection(Position *mousePos, int turn);
    extern void Position_updateDirection(Position *mousePos, int turn);
    extern int isCenter(Position *p);

