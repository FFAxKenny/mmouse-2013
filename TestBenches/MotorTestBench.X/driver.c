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
#include "motor.h"
#include "pinconfig.h"

#define TRUE 1
#define FALSE 0 

#define L90_SENSOR  0
#define R90_SENSOR  1
#define L45_SENSOR  2
#define R45_SENSOR  3
#define F1_SENSOR   4
#define F2_SENSOR   5

#define CELL_DISTANCE 1000

_FOSCSEL(FNOSC_FRC & IESO_OFF);
_FOSC(FCKSM_CSECMD & OSCIOFNC_OFF & POSCMD_NONE);

long i = 0;
int correct_offset = 0;

void delayMicro(unsigned int delay);

void initAD(void);
void initPLL(void);
void initTimer1(void);
void initPins(void);
void initMotors(void);
void sampleAD(void);

void Motor_step(Motor *m);
void Motor_init(Motor *m);
void Motor_enable(Motor *m);
void Motor_disable(Motor *m);

int sampleSensor(int sensor);

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

    while( sampleSensor(L45_SENSOR) < 50 );         // Wait for input
    for(k = 0; k< 150000; k++);                     // Delay

    /********************************
     *      Main Body 
     ********************************/ 
    LATBbits.LATB14 = 0;     // Enable Motors
    T1CONbits.TON = 1;       // Enable Timer

    while(1)
    {
        ADCValue = sampleSensor(L45_SENSOR);

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
void Motor_step(Motor *m)
{
    if(m->step == 1)
        m->step = 0;
    else
        m->step = 1;
    m->count++;
}
void Motor_enable(Motor *m)
{
    m->enable = 1;
}
void Motor_disable(Motor *m)
{
    m->enable = 0;
}

    

int calcError(int value1, int value2)
{

    return value1 - value2;

}


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


int sampleSensor(int sensor)
{
    AD1CON1bits.ADON = 0;
    switch(sensor)
    {
        case R45_SENSOR:
            AD1CHS0 = 0x0005;               
            __PIN_EmitR45 = 1;
            AD1CON1bits.ADON = 1;
            sampleAD();
            __PIN_EmitR45 = 0;
            break;
        case R90_SENSOR:
            AD1CHS0 = 0x0005;               
            __PIN_EmitR90 = 1;
            AD1CON1bits.ADON = 1;
            sampleAD();
            __PIN_EmitR90 = 0;
            break;
        case L45_SENSOR:
            AD1CHS0 = 0x0004;               
            __PIN_EmitL45 = 1;
            AD1CON1bits.ADON = 1;
            sampleAD();
            __PIN_EmitL45 = 0;
            break;
        case L90_SENSOR:
            AD1CHS0 = 0x0005;               
            __PIN_EmitL90 = 1;
            AD1CON1bits.ADON = 1;
            sampleAD();
            __PIN_EmitL90 = 0;
            break;
        case F1_SENSOR:
            AD1CHS0 = 0x0005;               
            __PIN_EmitF1 = 1;
            AD1CON1bits.ADON = 1;
            sampleAD();
            __PIN_EmitF1 = 0;
            break;
        case F2_SENSOR:
            AD1CHS0 = 0x0005;               
            __PIN_EmitF2 = 1;
            AD1CON1bits.ADON = 1;
            sampleAD();
            __PIN_EmitF2 = 0;
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
void Motor_init(Motor *m)
{
    m->step=1;
    m->enable=1;
    m->count=0;
}

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
    TRISAbits.TRISA0  = 1;   // Configure A0 as an input   
    
    LATBbits.LATB5 = 1;
    LATBbits.LATB6 = 1;
    LATBbits.LATB14 = 1;
}

void initAD(void)
{
    /**************************************
     *      Analog to Digital Configuration
     **************************************/
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

