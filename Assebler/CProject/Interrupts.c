/*
* GccApplication1.c
*
* Created: 2013-11-13 16:13:05
*  Author: student
*/


#define F_CPU  16000000

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/delay.h>


// Timeout in us (0-126)
void SETUP_TIMEOUT( unsigned char us )
{
	// send interrput on compare
	TIMSK|=(1<<OCIE0);
	// prescaler x8
	TCCR0|=(1<<CS01);
	// CTC
	TCCR0|=(1<<WGM01);

	// x2 (x16 with prescaler)
	OCR0 = us<<1;

	sei();
}


void SETUP_PWM()
{
	// prescaler 1x
	TCCR0|=(1<<CS00);
	// Fast PWM
	TCCR0|=(1<<WGM01)|(1<<WGM00);
	// clear COC on compare, set on bottom
	TCCR0|=(1<<COM01);

	// set COC on to out
	DDRB |= (1<<PB3);
}

void SET_PWM(unsigned char us)
{
	OCR0 = us;
}

ISR(TIMER0_COMP_vect)
{
	//PORTB += (1<<PB3);
}




int main(void)
{
	//SETUP_TIMEOUT(10);
	SETUP_PWM();
	unsigned char i = 0;
	while(1)
	{
		SET_PWM(++i);
		_delay_ms(4);
	}
}