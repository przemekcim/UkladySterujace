#include "serial.h" 

char serial_inBuffor[80];
char serial_outBuffor[80];
uint8_t serial_inCounter;
char* serial_toSend;

void (*inCall)(char*);

void serial_init(void (*inCallback)(char*)) 
{
	serial_inCounter = 0;
	serial_toSend = 0;
	inCall = inCallback;
	
	// Set baud rate
	UBRRH = 0;
	UBRRL = 103;
	
	// Enable receiver and transmitter
	UCSRB = (1<<RXEN)|(1<<TXEN)|(1<<RXCIE)|(1<<TXCIE);
	
	// Set frame format: 8data, 1stop bit
	UCSRC = (1<<URSEL)|(1<<UCSZ0)|(1<<UCSZ1);
	
	sei();
}

extern int serial_send(const char* val)
{
	if(serial_toSend)
		return 1;
		
	uint8_t i;
	for(i = 0; val[i] && i<79; ++i)
		serial_outBuffor[i] = val[i];
	serial_outBuffor[i] = 0;
	
	serial_toSend = serial_outBuffor;
	
	++serial_toSend;
	UDR = *(serial_toSend-1);
	
	return 0;
}

ISR(USART_TXC_vect)
{
	if(*serial_toSend == 0)
	{
		serial_toSend = 0;
	} else
	{
		++serial_toSend;
		UDR = *(serial_toSend-1);
	}
}

// on receive
ISR(USART_RXC_vect)
{
	uint8_t transmit = UDR;
	++serial_inCounter;
	
	//TODO
	UDR = transmit;
	
	if(transmit == '\r') {
		serial_inBuffor[serial_inCounter-1] = 0;
		cli();
		
		inCall(serial_inBuffor);
		
		sei();
		serial_inCounter = 0;
	} else 
		serial_inBuffor[serial_inCounter-1] = transmit;
}

uint8_t getSize(const char* msg)
{
	uint8_t size = 0;
	while(*msg)
	{
		++size;
		++msg;
	}
	
	return size;
}