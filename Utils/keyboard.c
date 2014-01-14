#include "keyboard.h"

uint8_t readKeyboard()
{
	uint8_t val = 0;
	
	PORTC = 0;
	DDRC = 0xF0;
	PORTC = 0x0F;
	for(uint8_t i = 0; i<10; ++i);
	val |= PINC & 0x0F;
	
	PORTC = 0;
	DDRC = 0x0F;
	PORTC = 0xF0;
	for(uint8_t i = 0; i<10; ++i);
	val |= PINC & 0xF0;
	
	return val;
}

uint8_t keyboardToInt(uint8_t val)
{
	uint8_t x = 0;
	uint8_t y = 0;
	val = ~val;
	
	for(uint8_t i=0; i<4; ++i)
	{
		if( (val>>i) & 1 )
			x = i;
		if( (val>>(i+4)) & 1 )
			y = i+1;
	}
	
	return x * 4 + y;
}