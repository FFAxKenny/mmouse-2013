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


_FOSCSEL(FNOSC_FRC & IESO_OFF);
_FOSC(FCKSM_CSECMD & OSCIOFNC_OFF & POSCMD_NONE);
//_FWDT(FWDTEN_OFF);
//_FPOR(FPWRT_PWR128 & BOREN_ON & ALTI2C1_ON & ALTI2C2_ON);
//_FICD(ICS_PGD1 & RSTPRI_PF & JTAGEN_OFF);

long i = 0;
int motor_pulse = 0;
int test;
float value;
int corr;
int ADCValue;

void delayMicro(unsigned int delay);
void initAD(void);
void initPLL(void);
void initTimer1(void);

int main()
{
    double k = 40000;
    //int number = 0;

    LATBbits.LATB9=1;
    LATBbits.LATB6=1;
    LATBbits.LATB5=1;
    LATAbits.LATA4=1;

    
    ANSELA = 0;//configures pin B4 as digital
    ANSELB = 0;//configures pin B4 as digital
 

    TRISA = 0b00000000;      // Configure B Ports as output
    TRISB = 0b00000000;      // Configure B Ports as output
    TRISBbits.TRISB15 = 1;
    TRISAbits.TRISA0  = 1;   // Configure A0 as an input   
    
    LATBbits.LATB5 = 1;
    LATBbits.LATB6 = 1;
    LATBbits.LATB14 = 1;
    

    initTimer1();
    initPLL();
    initAD();

    while(PORTBbits.RB15 == 0);
    for(k = 0; k< 30000; k++);
    /********************************
     *      Main Body
     ********************************/
    LATBbits.LATB14 = 0;
    T1CONbits.TON = 1;       // Enable Timer

    while(1)
    {

        /*
        delayMicro(100);
        AD1CON1bits.SAMP = 0;
        while (!AD1CON1bits.DONE);
        AD1CON1bits.DONE = 0;
        ADCValue = ADC1BUF0;

        LATBbits.LATB13 = 1;
        if(ADCValue > 100) 
        {
            LATBbits.LATB14 = 1;
        }
        else
        {
            LATBbits.LATB14 = 0;
        }

            __delay32(k);
            LATBbits.LATB9 = 1;
            LATBbits.LATB8 = 1;
            __delay32(k);
            LATBbits.LATB9 = 0;
            LATBbits.LATB8 = 0;
            if(k>10000) k=k-100;
         */
    }


}

void __attribute__((__interrupt__, __auto_psv__)) _T1Interrupt(void)
{
        _T1IF = 0;      // Reset the timer flag

        if(motor_pulse==0)
            motor_pulse=1;
        else
            motor_pulse=0;

        LATBbits.LATB9 = motor_pulse;
        LATBbits.LATB8 = motor_pulse;

        
        if(PR1 > 4000)
            PR1 = PR1 - 25;            // Set the timer

        /*
        else if(PR1 > 2200)
        {
            PR1 = PR1 - 5;
        } else
        {
        
        }
        else if(PR1 > 2000)
        {
            if( test == 1 )
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


void initAD(void)
{
    /**************************************
     *      Analog to Digital Configuration
     **************************************/
    ANSELAbits.ANSA0 = 1;         // Set pin A0 as analog 
    ANSELAbits.ANSA1 = 1;         // Set pin A0 as analog 

    AD1CON1 = 0x0004;
    AD1CON2 = 0x0000;
    AD1CON3 = 0x000F;
    AD1CON4 = 0x0000;
    AD1CHS0 = 0x0001;
    AD1CHS123 = 0x0000;
    AD1CSSH = 0x0000;
    AD1CSSL = 0x0000;
    AD1CON1bits.ADON = 1;

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
    PR1 = 17000;             // Set the timer

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
