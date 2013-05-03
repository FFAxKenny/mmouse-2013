/**********************************************************************
* © 2010 Marco D'Ambrosio / Giovanni Bernardo
*
* MuIn dsPIC, UART1 usage example
*
* v 1.0 06/09/2010
* 
**********************************************************************/

#include "common.h"
#include "uart_init.c"

_FOSCSEL(FNOSC_PRIPLL);
_FOSC(FCKSM_CSDCMD & OSCIOFNC_OFF & POSCMD_XT);
_FWDT(FWDTEN_OFF);

//#define LED1 	_LATA4
//#define LED2 	_LATB4

char HelloString[]={"\n\rHello World!"};
int i;

int main (void)
	{
	LATA = 0x0000; 
	LATB = 0x0000;
	TRISA = 0b1111111111101111; 
	TRISB = 0b1111111111101111;
	AD1PCFGL = 0x03C0; // All analog pins as digital
	OSC_init(); // initialize oscillator
	Uart_init(); // initialize UART
	Gen_init(); // general system init

	while (1) 
		{ 
		for(i=0;i<16;i++)
			{
			U1TXREG = HelloString[i];
			while(U1STAbits.UTXBF); // Wait until TX buffer is read for new data, when UTXBF=1 => buffer is full
			}
		}
	return(0);
	}
	
