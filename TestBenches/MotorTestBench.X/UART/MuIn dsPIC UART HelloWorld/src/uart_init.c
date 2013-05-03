long int BaudRate;
int BRG;

void Uart_init(void)
	{
	BaudRate = 57600; // Desired BaudRate
	BRG = (FCY/(4*(BaudRate)))-0.5; 
	U1MODE=0b1000000000001000; // UART1 Enabled, High BRG
	U1STA=0b0000010000000000; // TX Enabled
	U1BRG = BRG;
	}
	
