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

void configurePLL(void);
void configureTimer(void);

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


    configurePLL();
    configureTimer();

    while(1)
    {
        i=0;
        while(i<80000) {
            LATBbits.LATB4 = 1;
            LATAbits.LATA4 = 1;
            i++;
        }


        i = 0;
        while(i<70000) {
            LATBbits.LATB4 = 0;
            LATAbits.LATA4 = 0;
            i++;
        }

    }
    while(1);




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
    T1CON = 0;
    T1CONbits.TCKPS = 1;
    PR1 = 50000;

    _T1IP = 1;
    _T1IF = 0;
    _T1IE = 1;
    T1CONbits.TON = 1;

}

void __attribute__((__interrupt__, __auto_psv__)) _T1Interrupt(void)
{
        i=0;
        while(i<80000) {
            LATBbits.LATB4 = 1;
            LATAbits.LATA4 = 1;
            i++;
        }

        i = 0;
        while(i<70000) {
            LATBbits.LATB4 = 0;
            LATAbits.LATA4 = 0;
            i++;
        }

}
