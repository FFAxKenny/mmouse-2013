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
#define __ON    1
#define __OFF   0

// Definitions for Analog to Digital Conversion
#define L90_SENSOR  0
#define R90_SENSOR  1
#define L45_SENSOR  2
#define R45_SENSOR  3
#define F1_SENSOR   4
#define F2_SENSOR   5

#define CELL_DISTANCE 1000

// Configuration Options
_FOSCSEL(FNOSC_FRC & IESO_OFF);                     // Select Oscillator
_FOSC(FCKSM_CSECMD & OSCIOFNC_ON & POSCMD_NONE);   // Some other stuff

long i = 0;
int correct_offset = 0;

void allEmitters(int state);
void powerMotors(int state);
void powerEmitters(int state);

void initAD(void);                                  // Init Functions
void initPLL(void);
void initTimer1(void);
void initPins(void);
void initMotors(void);
int sampleSensor(int sensor);                       // Analog to digital
void sampleAD(void);
void delayMicro(unsigned int delay);                // Misc

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
    initPLL();
    initAD();
    
    waitForStart();                                 // Wait for the start input
    for(k = 0; k< 150000; k++);                     // Delay 

    powerMotors(__ON);
    T1CONbits.TON = 1;                              // Enable Timer

    /********************************
     *      Main Body 
     ********************************/ 
    while(1)
    {
        ADCValue = sampleSensor(L90_SENSOR);
        while(lMotor.count < CELL_DISTANCE)
        {
            Motor_enable(&lMotor);
            Motor_enable(&rMotor);
        } 
        __asm__ volatile ("reset");

        // Software Reset
        if(PORTBbits.RB15 == 1)
            __asm__ volatile ("reset");

    }


}



/*********************************************************************
 *      Interrupt Service Routine 1
 *********************************************************************/ 
void __attribute__((__interrupt__, __auto_psv__)) _T1Interrupt(void){
        _T1IF = 0;      // Reset the timer flag
        
        if( lMotor.enable || correct_offset < 25)
            Motor_step(&lMotor);

        if( rMotor.enable || correct_offset < 25)
            Motor_step(&rMotor);

        if(correct_offset < 25)
            correct_offset++;
        else
            correct_offset = 0;

        __PIN_MotorRStep = rMotor.step;   // Update right motor state
        __PIN_MotorLStep = lMotor.step;   // Update left motor state

        // B9 is the right motor
        // B8 is the left motor
}
/*********************************************************************
 *      Routine Functions
 *********************************************************************/ 
inline void waitForStart(void){
    while( sampleSensor(R90_SENSOR) < 500 );         // Wait for start input
}
inline void powerEmitters(int state){

}
inline void powerMotors(int state){
    LATBbits.LATB14 = 0;                            // Enable Motors
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
    delayMicro(100);
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
    allEmitters(__OFF);
    
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
    PR1 = 7000;             // Set the timer

    _T1IP = 1;
    _T1IF = 0;
    _T1IE = 1;
    T1CONbits.TON = 0;       // Enable Timer

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

