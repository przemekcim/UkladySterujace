#include "serial.h" 


void serial_init() 
{
	// Set baud rate
	UBRRH = 0;
	UBRRL = 103;
	
	// Enable receiver and transmitter
	UCSRB = (1<<RXEN)|(1<<TXEN)|(1<<RXCIE)|(1<<TXCIE);
	
	// Set frame format: 8data, 1stop bit
	UCSRC = (1<<URSEL)|(1<<UCSZ0)|(1<<UCSZ1);
	
	sei();
}