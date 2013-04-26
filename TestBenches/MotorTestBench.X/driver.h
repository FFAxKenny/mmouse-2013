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

    /* Variables */
    int error = 0;
    int prevError = 0;
    int right = 0;
    int left = 0;
    int front = 0;
    int speedValue = 8000;

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
    int abs (int n);
