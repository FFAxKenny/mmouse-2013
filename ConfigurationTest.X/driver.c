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


_FOSCSEL(FNOSC_FRC & IESO_OFF);
_FOSC(FCKSM_CSECMD & OSCIOFNC_OFF & POSCMD_NONE);

long i = 0;
int main()
{
    long i = 0;
    //int number = 0;

    LATBbits.LATB4=0;
    LATAbits.LATA4=0;

    ANSELA = 0;//configures pin B4 as digital
    ANSELB = 0;//configures pin B4 as digital

    TRISA = 0b00000000;      // Configure B Ports as output
    TRISB = 0b00000000;      // Configure B Ports as output

 
    /********************************
     *      Timer1 Configuration
     ********************************/
    T1CON = 0;               // Reset T1 Configuration
    T1CONbits.TCKPS = 3;     // Set the ratio to the highest
    PR1 = 50000;             // Set the timer

    _T1IP = 1;
    _T1IF = 0;
    _T1IE = 1;
    T1CONbits.TON = 1;       // Enable Timer

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


    // This bit of code doesn't work for some reason..
    /**************************************
        // Wait for Clock switch to occur
        while (OSCCONbits.COSC!= 0b001);
        

        // Wait for PLL to lock
        while (OSCCONbits.LOCK!= 1);
    ***************************************/


    /********************************
     *      Main Body
     ********************************/

    while(1)
    {
            __delay32(50000);
            LATAbits.LATA4 = 1;
            __delay32(50000);
            LATAbits.LATA4 = 0;
    }



}

void __attribute__((__interrupt__, __auto_psv__)) _T1Interrupt(void)
{
        
        _T1IF = 0;      // Reset the timer flag
        
        // The body of the Timer1 Interrupt goes here

}

