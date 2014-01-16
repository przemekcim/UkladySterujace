#include "scheduler.h"
#include "display.h"
#include "serial.h"
#include "HD44780.h"

//static volatile uint8_t& port = DDRA;
uint8_t display[4];

void onReceive(char* msg)
{
	uint8_t size = 0;
	while(*msg) 
	{
		++size;
		++msg;
	}
	
	binToDecDigits(size, display, 4);
}

void sendText()
{
	serial_send("Hello");
};

int main(void)
{
	binToDecDigits(1234, display, 4);
	
    scheduler_init(display);
	serial_init(onReceive);
	
	scheduler_addPeriodicTask(sendText, 5000);
	
	LCD_Initalize();
	LCD_Command(HD44780_DISPLAY_ONOFF|HD44780_DISPLAY_ON);
	LCD_Text("Ala ma kota!");
	
	scheduler_run();
}