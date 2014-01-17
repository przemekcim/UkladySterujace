#include "HD44780.h"



void WriteNibble(unsigned char nibbleToWrite)
{
	//set E - 4-th bit
	PORTD |= E;
	
	// clear oldest 4 bits
	PORTD &= 0x0f;
	// set oldest 4 bits
	PORTD |= nibbleToWrite<<4;
	// clear E
	PORTD &= ~E;
}


void WriteByte(unsigned char dataToWrite)
{
	WriteNibble(dataToWrite>>4);
	WriteNibble(dataToWrite);
	_delay_us(50);
}


void LCD_Command(unsigned char a)
{
	PORTD &= ~RS;
	WriteByte(a);
};

void LCD_Text(const char *a)
{
	PORTD |= RS;
	while(*a)
	{
		WriteByte(*a);
		++a;
	}
};
void LCD_GoToXY(unsigned char a, unsigned char b)
{
};

void LCD_Clear(void)
{
};

void LCD_Home(void)
{
};

void LCD_Initalize(void)
{
	DDRD |= 0xfc;
	PORTD &= ~0xfc;

	_delay_ms(20);
	WriteNibble(3);
	_delay_ms(16);
	WriteNibble(3);
	_delay_ms(16);
	WriteNibble(3);
	_delay_ms(16);
	WriteNibble(2);
	_delay_us(50);

	LCD_Command(HD44780_FUNCTION_SET|HD44780_FONT5x7|HD44780_TWO_LINE);

	LCD_Command(HD44780_DISPLAY_ONOFF|HD44780_DISPLAY_OFF);
	LCD_Command(HD44780_CLEAR);
	_delay_ms(100);
	LCD_Command(HD44780_ENTRY_MODE|HD44780_EM_INCREMENT|HD44780_EM_SHIFT_CURSOR);
};