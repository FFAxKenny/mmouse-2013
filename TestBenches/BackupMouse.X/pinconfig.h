// Pin Configuration Definitions


// Motor Ports
#define __PIN_MotorRStep        LATBbits.LATB9
#define __PIN_MotorRDir         LATBbits.LATB5

#define __PIN_MotorLStep        LATBbits.LATB8
#define __PIN_MotorLDir         LATBbits.LATB6

// Emitters
#define __PIN_EmitR45            LATBbits.LATB7
#define __PIN_EmitR90            LATAbits.LATA4
#define __PIN_EmitL45            LATAbits.LATA4
#define __PIN_EmitL90            LATAbits.LATA2
#define __PIN_EmitF1             LATAbits.LATA3
#define __PIN_EmitF2             LATBbits.LATB4

// Detectors
#define __PIN_DetectR45
#define __PIN_DetectR90
#define __PIN_DetectL45
#define __PIN_DetectL90
#define __PIN_DetectF1
#define __PIN_DetectF1

// Xbee
#define __PIN_XBeeRX
#define __PIN_XBeeTX

// DIP Switch Input
#define __PIN_DIP_Input1
#define __PIN_DIP_Input2
#define __PIN_DIP_Input3
