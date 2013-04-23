/* * File:   driver.c
 * Author: kenny
 *
 * Created on February 9, 2013, 11:10 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include <dsp.h>
#include <p33Exxxx.h>

// User Created H Files
#include "motor.h"
#include "pinconfig.h"

// True False Definitions
#define TRUE    1
#define FALSE   0 
#define ON      1
#define OFF     0

#define LEFT    0
#define RIGHT   1

// Definitions for Analog to Digital Conversion
#define L90_SENSOR  0
#define R90_SENSOR  1
#define L45_SENSOR  2
#define R45_SENSOR  3
#define F1_SENSOR   4
#define F2_SENSOR   5

#define SENSOR_OFFSET 

#define CELL_DISTANCE 363
#define DISTANCE_360 240
#define DISTANCE_90  116

#define RIGHT_THRESHOLD         30
#define LEFT_THRESHOLD          30
#define NOMINAL_RIGHT_VALUE     130
#define NOMINAL_LEFT_VALUE      141

#pragma config ICS = PGD2

// Configuration Options
_FOSCSEL(FNOSC_FRC & IESO_OFF);                     // Select Oscillator
_FOSC(FCKSM_CSECMD & OSCIOFNC_ON & POSCMD_NONE);   // Some other stuff

int CORRECT_DISTANCE = 10;
long i = 0;
int correct_offset = 0;
int error = 0;

int right = 0;
int left = 0;
int front = 0;
int front_left = 0;
int front_right = 0;

int correct_interval = 5;

void allEmitters(int state);
void powerMotors(int state);
void powerEmitters(int state);

void moveCell(int n);

void initAD(void);                                  // Init Functions
void initPLL(void);
void initTimer1(void);
void initTimer2(void);
void initPins(void);
void initMotors(void);
int sampleSensor(int sensor);                       // Analog to digital
void sampleAD(void);
void delayMicro(unsigned int delay);                // Misc

void turn90(int direction);
void turn180(int direction);

void realign(int n);

inline void enableTimer(int n);
inline void disableTimer(int n);
inline void waitForStart(void);
inline void powerEmitters(int state);
inline void powerMotors(int state);
int prevError = 0;

void updateMotorStates(void);

int correctCount = 0;

int errorOffset = 0;

int nominalRightValue = 0;
int nominalLeftValue = 0;

void sampleAllSensors();

// Declare the motors
Motor lMotor;
Motor rMotor;

int main()
{
    double k;
    int ADCValue;

    // Init stuff
    initMotors();
    initPins();
    initTimer1();
    initTimer2();
    initPLL();
    initAD();

    LATBbits.LATB13 = 1;                            // Enable Motors

    waitForStart();                                 // Wait for the start input
    for(k = 0; k< 150000; k++);                     // Delay 
   
    // Clear the Buffer 
    ADC1BUF0 = 0;
    nominalLeftValue = 133;
    nominalRightValue = 125;
    left = sampleSensor(L90_SENSOR);
    right = sampleSensor(R90_SENSOR);
    front = sampleSensor(F1_SENSOR);
    powerMotors(ON);

    
    /********************************
     *      Wall Hugger Algorithm
     ********************************/ 
    while(1) {
        moveCell(1);
    }
}

void sampleAllSensors(){
    left = sampleSensor(L90_SENSOR);
    right = sampleSensor(R90_SENSOR);
    front = sampleSensor(F1_SENSOR);
}

void moveCell(int n)
{
    double temp;
    temp = lMotor.count;
    int error = right - 120;
    int pK = 2;
    int pD = 60;
    error = error;
    sampleAllSensors();

    while( (lMotor.count - temp) < 3) {
            disableTimer(1);
            disableTimer(2);
            _T1IF = 0;
            _T2IF = 0;
            PR1 = 10000 - error*pK -  (error-prevError)*pD;  // Left Motor
            PR2 = 10000 + error*pK + (error-prevError)*pD;  // Right Motor
            enableTimer(1);
            enableTimer(2);
            prevError = error;
    }

}

void turn90(int direction)
{
    int temp;
    int yay;
    int pay;
    int j;
    PR1 = 11000;             // Set the timer

    if(direction == RIGHT){
        lMotor.dir = 0;
        rMotor.dir = 1;
    }
    else if(direction == LEFT){
        lMotor.dir = 1;
        rMotor.dir = 0;
    }

    Motor_disable(&lMotor);
    Motor_disable(&rMotor);

    __delay32(5000000);
    __delay32(5000000);
    __delay32(5000000);


    Motor_enable(&lMotor);
    Motor_enable(&rMotor);

    temp = lMotor.count;
    while( (lMotor.count - temp ) < DISTANCE_90);
    lMotor.dir = 1;
    rMotor.dir = 1;

    Motor_disable(&lMotor);
    Motor_disable(&rMotor);
    __delay32(5000000);
    __delay32(5000000);
    __delay32(5000000);
    Motor_enable(&lMotor);
    Motor_enable(&rMotor);

    int temp9 = lMotor.count;
    while((lMotor.count - temp9) < 70)
    {
        lMotor.dir = 0;
        rMotor.dir = 0;
    }

    lMotor.dir = 1;
    rMotor.dir = 1;
    
    Motor_disable(&lMotor);
    Motor_disable(&rMotor);
    __delay32(5000000);
    __delay32(5000000);
    __delay32(5000000);
    Motor_enable(&lMotor);
    Motor_enable(&rMotor);


    PR1 = 12000;             // Set the timer
}

void turn180(int direction)
{
    turn90(direction);
    turn90(direction);
}

void enableTimer(int n)
{
    switch(n)
    {
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

void disableTimer(int n)
{
    switch(n)
    {
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
        if(_T2IF == 1) _T2IF = 0;
        if( lMotor.enable)
            Motor_step(&lMotor);
        __PIN_MotorLStep = lMotor.step;   // Update left motor state
        __PIN_MotorLDir = lMotor.dir;
}
void __attribute__((__interrupt__, __auto_psv__)) _T2Interrupt(void){
        _T2IF = 0;      // Reset the timer flag
        if(_T1IF == 1) _T1IF = 0;

        if( rMotor.enable ) 
            Motor_step(&rMotor);
        __PIN_MotorRStep = rMotor.step;   // Update right motor state
        __PIN_MotorRDir = rMotor.dir;
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
    delayMicro(10);
    AD1CON1bits.SAMP = 0;
    while (!AD1CON1bits.DONE);
    AD1CON1bits.DONE = 0;
}


/*********************************************************************
 *     Initialization Functions 
 *********************************************************************/ 
void initMotors(void)
{
    Motor_init(&lMotor);
    Motor_init(&rMotor);
}
void initPins(void)
{
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
void allEmitters(int state)
{
    LATAbits.LATA2 = state;             // L90
    LATAbits.LATA3 = state;             // FL
    LATBbits.LATB4 = state;             // FR
    LATAbits.LATA4 = state;             // R90
    LATBbits.LATB4 = state;
    LATBbits.LATB10 = state;
}



void initAD(void)
{
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


void initPLL(void)
{
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

void initTimer1(void)
{
    /********************************
     *      Timer1 Configuration
     ********************************/
    T1CON = 0;               // Reset T1 Configuration
    T1CONbits.TCKPS = 1;     // Set the ratio to the highest
    PR1 = 9000;             // Set the timer

    _T1IP = 1;
    _T1IF = 0;
    _T1IE = 1;
    T1CONbits.TON = 0;       // Enable Timer

}
void initTimer2(void)
{
    /********************************
     *      Timer2 Configuration
     ********************************/
    T2CON = 0;               // Reset T1 Configuration
    T2CONbits.TCKPS = 1;     // Set the ratio to the highest
    PR2 = 9000;             // Set the timer

    _T2IP = 1;
    _T2IF = 0;
    _T2IE = 1;
    T2CONbits.TON = 0;       // Enable Timer
}

/*********************************************************************
 *     Initialization Functions 
 *********************************************************************/ 
void delayMicro(unsigned int delay)
{

    for (i = 0; i < delay; i++)
    {
    __asm__ volatile ("repeat #39");
    __asm__ volatile ("nop");
    }

}


void acceleration(void)
{



    // Goes in the timer code 
        //if(PR1 > 6000)
            //PR1 = PR1 - 25;            // Set the timer

        /*
        else if(PR1 > 2200)
        {
            PR1 = PR1 - 5;
        } 
        else if(PR1 > 1800)
        {
            if( test == 45 )
            {
                PR1 = PR1-1;
                test = 0;
            }
            else
            {
                test++;
            }
        }
        */
        

        // The body of the Timer1 Interrupt goes here

}

