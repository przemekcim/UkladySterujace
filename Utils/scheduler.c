#include "scheduler.h"

Task scheduler_tasks[MAX_TASK];
uint8_t scheduler_taskCount;
uint8_t scheduler_displayCount;
uint8_t* scheduler_display;


void scheduler_init(uint8_t* display)
{
	// send interrput on compare
	TIMSK|=(1<<OCIE0);
	// prescaler x64
	TCCR0|=(1<<CS00) | (1<<CS01);
	OCR0 = 250;
	// 64 * 250 = 16k = 1ms
	// CTC
	TCCR0|=(1<<WGM01);
	
	// init with default values
	scheduler_taskCount = 0;
	for(uint8_t i = 0; i<MAX_TASK; ++i)
	{
		scheduler_tasks[i].interval = NON_PERIODIC;
		scheduler_tasks[i].ready = 0;
		scheduler_tasks[i].task = 0;
		scheduler_tasks[i].toGo = 0;
	}
	
	scheduler_display = display;
	scheduler_displayCount = 0;
}

void scheduler_run()
{
	// set interrupts
	sei();
	uint8_t i = 0;

	while(scheduler_taskCount)
	{
		// if task is ready
		if(scheduler_tasks[i].ready)
		{
			--(scheduler_tasks[i].ready);
			// run task
			scheduler_tasks[i].task();
			// start from begining - closer to the array begin, greater priority
			i = 0;
		} else {
			++i;
			if(i >= scheduler_taskCount)
				i=0;
		}
	}
}

void scheduler_addPeriodicTask(TASK_PTR task, uint16_t interval)
{
	scheduler_tasks[scheduler_taskCount].task = task;
	scheduler_tasks[scheduler_taskCount].interval = interval;
	scheduler_tasks[scheduler_taskCount].toGo = interval;
	scheduler_tasks[scheduler_taskCount].ready = 0;
	++scheduler_taskCount;
}

void scheduler_addOneShotTask(TASK_PTR task, uint16_t interval)
{
	scheduler_tasks[scheduler_taskCount].task = task;
	scheduler_tasks[scheduler_taskCount].interval = NON_PERIODIC;
	scheduler_tasks[scheduler_taskCount].toGo = interval;
	scheduler_tasks[scheduler_taskCount].ready = 0;
	++scheduler_taskCount;
}


ISR(TIMER0_COMP_vect)
{
	// increase counter for displayer
	++scheduler_displayCount;
	// refresh displayer every 4 ms == every 4 interrupts
	// to do so we check if last two bits of counter equal 0
	if(scheduler_display && ((scheduler_displayCount&3)==0))
	{
		PORTB &= 0xf0;
		// 3rd and 4th bit of counter indicates which (of 4) displayer are being refreshed
		uint8_t toRefresh = (scheduler_displayCount>>2) & 0x3;
		PORTA = scheduler_display[toRefresh];
		PORTB =~(1<<(toRefresh);
	}	
	
	// for each task
	for(uint8_t i = 0; i<scheduler_taskCount; ++i)
	{
		if(scheduler_tasks[i].interval != NON_PERIODIC)
		{
			--(scheduler_tasks[i].toGo);
			if(scheduler_tasks[i].toGo == 0)
			{
				++(scheduler_tasks[i].ready);
				scheduler_tasks[i].toGo = scheduler_tasks[i].interval;
			}
		} else {
			if(scheduler_tasks[i].toGo > 0)
			{
				--(scheduler_tasks[i].toGo);
				if(scheduler_tasks[i].toGo==0)
					scheduler_tasks[i].ready = 1;
			}
		}
	}
}
