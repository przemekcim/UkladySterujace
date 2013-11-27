#define F_CPU 16000000L

#include "Scheduler.h"
#include <avr/delay.h>

const uint8_t DIGITS[16] = {~0x3F, ~0x06, ~0x5B, ~0x4F, ~0x66, ~0x6D, ~0x7D, ~0x07, ~0x7F, ~0x6F, ~0x77, ~0x7C, ~0x39, ~0x5e, ~0x79, ~0x71};
const uint8_t DIGIT_DOT = 0x7f;


uint8_t values[4];
uint8_t values_counter = 0;
uint16_t second_counter = 0;
void taskShowDigits() 
{
	PORTB &= 0xf0;
	PORTA = values[values_counter & 0x3];
	PORTB = ~(1<<(values_counter & 0x3));
	++values_counter;
}

void binToDecDigits(uint16_t val, uint8_t* tab, uint8_t n)
{
	for(uint8_t i = 0; i<n; ++i) {
		tab[i] = DIGITS[val % 10];
		val /= 10;
	}
}

void taskDelay() 
{
	_delay_ms(3);
}

void taskAddValue() {
	binToDecDigits(++second_counter, values, 4);
	values[2] &= DIGIT_DOT;
}


int main(void)
{
	DDRA = 0xff;
	DDRB = 0x0f;
	
	binToDecDigits(0, values, 4);
	
    scheduler_init();
	scheduler_addPeriodicTask(taskDelay, 4);
	scheduler_addPeriodicTask(taskShowDigits, 4);
	scheduler_addPeriodicTask(taskAddValue, 10);
	scheduler_run();
}
