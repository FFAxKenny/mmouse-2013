#include "common.h"
#warning -- compiling for 990.020 board 

void Gen_init(void)
	{
	// Peripheral PIN selection ***********************************

	//************************************************************ 
	// Unlock Registers
	//*************************************************************
	asm volatile ( "mov #OSCCONL, w1 \n"
	"mov #0x45, w2 \n"
	"mov #0x57, w3 \n"
	"mov.b w2, [w1] \n"
	"mov.b w3, [w1] \n"
	"bclr OSCCON, #6 ");


	// insert your code for pin remapping

	// see this document for pin remapping:
	// http://ww1.microchip.com/downloads/en/DeviceDoc/70190C.pdf
	// "Section 30. I/O Ports with Peripheral Pin Select"

	// on 990.012 UARTs are connected in this manner:
	// U1TX on RB5	(RP5)
	// U1RX on RB6	(RP6)
	// U2TX on RB7	(RP7)
	// U2RX on RB10	(RP10)

	// Output functions are selected by RPINRxx registers
	// Input functions are selected by RPORx registers

	RPINR18bits.U1RXR = 6; // assign UART1RX to pin RP6 (RB6)

	// Output functions on pins RP5 and RP4 is selected by RPOR2 register
	// bits RP5R => specified function assigned to RP5 pin
	// bits RP4R => specified function assigned to RP4 pin
	// 0b00011 => U1TX function
	RPOR2bits.RP5R = 0b00011;


	//************************************************************
	// Lock Registers
	//************************************************************
	asm volatile ( "mov #OSCCONL, w1 \n"
	"mov #0x45, w2 \n"
	"mov #0x57, w3 \n"
	"mov.b w2, [w1] \n"
	"mov.b w3, [w1] \n"
	"bset OSCCON, #6");
 
	/*---------------------------------------------------------------------------*/
	/* Timer 1	1ms   			    									 */
	/*---------------------------------------------------------------------------*/
	#define TMR1_VALUE 40000
	OpenTimer1(T1_ON & 
			T1_GATE_OFF & 
			T1_PS_1_1 & 
			T1_SYNC_EXT_OFF &
			T1_SOURCE_INT, 
   			TMR1_VALUE);
 
	ConfigIntTimer1(T1_INT_PRIOR_4 & T1_INT_OFF);	// no interrupt	
	}
	
