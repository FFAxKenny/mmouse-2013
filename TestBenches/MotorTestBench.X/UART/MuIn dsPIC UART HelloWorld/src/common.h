extern void OSC_init (void);
extern void Gen_init(void);

#include "p33FJ64GP802.h" 
#include "p33Fxxxx.h"
//#include "libpic30.h"
//#include "dsp.h"
//#include "pwm.h"
//#include "pwm12.h"
//#include "uart.h"
//#include "qei.h"
//#include "adc.h"
#include "timer.h"
//#include "incap.h"
#include "ports.h"
//#include "dma.h"
//#include "math.h"
#include "stdlib.h"
//#include "libq.h"

/*
CLOCK_FREQ = 10 MHz
Fosc = CLOCK_FREQ*M/(N1*N2) = 10M*32/(2*2)=80Mhz
Fcy = Fosc / 2 = 40
*/
#define FCY 40000000
// TCY = 1 / FCY = 25 ns
#define TCY 1/((float)(FCY)
#define _ISR_PSV __attribute__((interrupt, auto_psv))

