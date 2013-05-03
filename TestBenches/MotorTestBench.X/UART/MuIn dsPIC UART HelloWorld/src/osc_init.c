#include "common.h"

void OSC_init(void)
{
/* CLKDIV */
_ROI = 0x00;		// Recover on Interrupt bit
_DOZE = 0x03;		// Processor Clock Reduction Select bits
_DOZEN = 0x00;		// DOZE Mode Enable bit
_FRCDIV = 0x00;		// Internal Fast RC Oscillator Postscaler bits
_PLLPOST = 0x00;	// PLL VCO Output Divider Select bits (N2)
_PLLPRE = 0x00;		// PLL Phase Detector Input Divider bits (N1)
_PLLDIV = 0x1E;		// PLL divider (M=32)
		
RCONbits.SWDTEN=0;	// Disable Watch Dog Timer

while (OSCCONbits.COSC != 0b011);	// Wait for Clock switch to occur	
while(OSCCONbits.LOCK!=1) {};		// Wait for PLL to lock

/* 
XTAL = 10 MHz

PLLPRE = 1/2
PLLPORT = 1/2

Fosc = Fin * ( PLLDIV / (PLLPRE*PLLPOST) ) 
Fcy = Fosc / 2

10 MHz * ( 32 / (2*2) ) = 10 MHz * 32/4 = 80 MHz
*/

}
