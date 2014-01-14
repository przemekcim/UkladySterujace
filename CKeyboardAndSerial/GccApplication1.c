#include <avr/io.h>

#include "scheduler.h"
#include "display.h"
#include "keyboard.h"
#include "serial.h"

uint16_t global;
uint8_t disp[4];

void task2()
{
	uint8_t val = keyboardToInt(readKeyboard());
	
	if (val)
	{
		binToDecDigits(val, disp, 4);
		UDR = 'a' + val;
	}
	
}


int main(void)
{
	serial_init();
	
	global = 0;
	binToDecDigits(global, disp, 4);
	
    scheduler_init(disp);
	scheduler_addPeriodicTask(task2, 100);
	scheduler_run();
}

ISR(USART_RXC_vect)
{
	uint8_t transmit = UDR;
	binToDecDigits(transmit, disp, 4);
	UDR = transmit;
}