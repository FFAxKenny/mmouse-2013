/*
 * File:   driver.c
 * Author: kenny
 *
 * Created on February 9, 2013, 11:10 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include <dsp.h>
#include "config.h"

_FOSCSEL(FNOSC_FRC & IESO_OFF);
_FOSC(FCKSM_CSECMD & OSCIOFNC_OFF & POSCMD_NONE);

void configurePLL(void);
void configureTimer(void);

long i = 0;
int ledState = 0;

int main()
{
    long i = 0;
    //int number = 0;

    ANSELA = 0;//configures pin B4 as digital
    ANSELB = 0;//configures pin B4 as digital

    TRISA = 0b00000000;      // Configure B Ports as output
    TRISB = 0b00000000;      // Configure B Ports as output
    
    // Initial pin states
    LATBbits.LATB4=0;
    LATAbits.LATA4=0;

    // Configure things
    configurePLL();
    configureTimer();

    while(1);           // Don't need this since we're only using timers..
}

void __attribute__((__interrupt__, __auto_psv__)) _T1Interrupt(void)
{
    _T1IF = 0;      // Reset the timer flag

    if(ledState == 0)
        ledState = 1;
    else
        ledState = 0;

    /* Pins that we need to test:
     * RA2
     * RA3
     * RB4
     * RB5
     * RB10
     * RB9
     * RB8
     * RB7
     * RB6
     */
    LATBbits.LATB4=ledState;
    LATBbits.LATB5=ledState;
    LATBbits.LATB6=ledState;
    LATBbits.LATB7=ledState;
    LATBbits.LATB8=ledState;
    LATBbits.LATB9=ledState;
    LATBbits.LATB10=ledState;
    LATAbits.LATA4=ledState;
}

void configurePLL()
{
    // Configure PLL prescaler, PLL postscaler, PLL divisor
    PLLFBD=63;              // M=65
    CLKDIVbits.PLLPOST=0;   // N2=2
    CLKDIVbits.PLLPRE=1;    // N1=3

    // Initiate Clock Switch to FRC oscillator with PLL (NOSC=0b001)
    __builtin_write_OSCCONH(0x01);
    __builtin_write_OSCCONL(OSCCON | 0x01);

    while (OSCCONbits.COSC!= 0b001);
    while (OSCCONbits.LOCK!= 1);
}


void configureTimer()
{
    // Timer 1 configuration
    T1CON = 0;                  // Reset T1 Configuration
    T1CONbits.TCKPS = 3;        // Set ratio to the highest
    PR1 = 30000;                // Set the timer to look for

    _T1IP = 1;                  
    _T1IF = 0;
    _T1IE = 1;
    T1CONbits.TON = 1;          // Enable Timer
}

