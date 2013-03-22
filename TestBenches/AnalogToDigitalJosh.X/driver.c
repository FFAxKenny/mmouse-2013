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
#include "pinconfig.h"
#include <p33Exxxx.h>

//#pragma config OSCIOFNC = ON            // OSC2 Pin Function bit (OSC2 is general purpose digital I/O pin)
_FOSCSEL(FNOSC_FRC & IESO_OFF);                     // Set Clock
_FOSC(FCKSM_CSECMD & OSCIOFNC_ON & POSCMD_NONE);    // Set Some other things..

void configurePLL(void);
void configureTimer(void);
void Delayms(unsigned int delay);
void initAdc1(void);
int sampleADC(void);

int ledState = 0;
int reading = 0;

int main()
{
    
    //int number = 0;

    ANSELA = 0;                     // configures pin B4 as digital
    ANSELB = 0;                     // configures pin B4 as digital
    ANSELAbits.ANSA0 = 1;           // Select AN0 is analog

    TRISA = 0b00000000;             // Configure B Ports as output
    TRISB = 0b00000000;             // Configure B Ports as output
    
    TRISBbits.TRISB15 = 1;          // Inputs
    TRISBbits.TRISB14 = 1;
    TRISBbits.TRISB13 = 1;

    TRISAbits.TRISA0 = 1;           // Configure A0 is an input
    

    // Configure things
    configurePLL();                 // Configure the clock and PLL
    configureTimer();               // Configure the timer

    initAdc1();                     // Init the ADC Config

    PIN_Emitter1 = 0;
    PIN_Emitter2 = 0;

    while(1)                        // Check ADC 
    {
        reading = sampleADC();
        while(1);
        if(reading >= 1)
        {
            PIN_Emitter1 = 1;
        }
        else
        {
            PIN_Emitter1 = 0;
        }
        PIN_Emitter2 = 1;
    }


        

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
     PIN_Motor1_Step = ledState;
     PIN_Motor1_Direction = ledState;
     PIN_Motor2_Step = ledState;
     PIN_Motor2_Direction = ledState;

}

int sampleADC(void)
{
    AD1CON1bits.SAMP = 1;           // Start Conversion
    ADC1BUF0 = 0x0000;              // Clear ADC1 Buffer
    while (!AD1CON1bits.DONE);      // Conversion done? Waiting for 1
    AD1CON1bits.DONE = 0;           // Clear conversion done status bit
    ADCValue = int (1/ADC1BUF0);
    return ADCValue;
}

void configurePLL()
{
    // Configure PLL prescaler, PLL postscaler, PLL divisor
    PLLFBD=63;                  // M=65
    CLKDIVbits.PLLPOST=0;       // N2=2
    CLKDIVbits.PLLPRE=1;        // N1=3

    // Initiate Clock Switch to FRC oscillator with PLL (NOSC=0b001)
    __builtin_write_OSCCONH(0x01);
    __builtin_write_OSCCONL(OSCCON | 0x01);

    while (OSCCONbits.COSC!= 0b001);
    while (OSCCONbits.LOCK!= 1);
}


void configureTimer()
{
    // Timer 1 configuration
    T1CON = 0;                      // Reset T1 Configuration
    T1CONbits.TCKPS = TIMER_RATIO;  // Set ratio to the highest
    PR1 = TIMER_DELAY;              // Set the timer to look for

    _T1IP = 1;                  
    _T1IF = 0;
    _T1IE = 1;
    T1CONbits.TON = 1;              // Enable Timer
}


void initAdc1()
{
    AD1CON1 = 0x04E0;                   // A/D Control Register 1
    AD1CON2 = 0x0000;                   // A/D Control Register 2
    AD1CON3bits.SAMC = 0b11000;         // A/D Control Register 3
    AD1CON3bits.ADCS = 8;
    AD1CON4 = 0x0000;                   // A/D Control Register 4

    AD1CSSLbits.CSS0 = 1;               // Select AN0 Sensor L2
    AD1CSSLbits.CSS1 = 0;
    AD1CSSLbits.CSS4 = 0;
    AD1CSSLbits.CSS5 = 0;

    AD1CON1bits.ADON = 1;               // Begin Sampling Sequence

    /* Old Configuration */
    /*
    // Set analog port to analog
    ANSELAbits.ANSA0 = 1;           // Select AN0 is analog

    // Initialize and enable ADC
    AD1CON1 = 0x0000;       
    AD1CON2 = 0x0000;
    AD1CON3 = 0x001F;               //..1111
    AD1CON4 = 0x0000;
    AD1CHS0 = 0x0005;               //..0101
    AD1CHS123 = 0x0000;
    AD1CSSH = 0x0000;
    AD1CSSL = 0x0000;
    AD1CON1bits.ADON = 1;
    */
    Delayms(30);


}

void Delayms(unsigned int delay)
{
    int i;
    for (i = 0; i < delay; i++)
    {
        __asm__ volatile ("repeat #39");
        __asm__ volatile ("nop");
    }
}
