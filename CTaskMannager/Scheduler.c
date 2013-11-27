#include "Scheduler.h"

Task scheduler_tasks[MAX_TASK];
uint8_t scheduler_taskCount;
uint8_t scheduler_displayCount;
uint8_t* scheduler_display;

void scheduler_init(uint8_t* display)
{
	// send interrput on compare
	TIMSK|=(1<<OCIE0);
	// prescaler x8
	TCCR0|=(1<<CS00) | (1<<CS01);
	OCR0 = 250;
	// CTC
	TCCR0|=(1<<WGM01);
	
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
	sei();
	uint8_t i = 0;

	while(scheduler_taskCount)
	{
		if(scheduler_tasks[i].ready)
		{
			--(scheduler_tasks[i].ready);
			scheduler_tasks[i].task();
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
	++scheduler_displayCount;
	if(scheduler_display && ((scheduler_displayCount&3)==0))
	{
		PORTB &= 0xf0;
		PORTA = scheduler_display[(scheduler_displayCount>>2) & 0x3];
		PORTB =~(1<<((scheduler_displayCount>>2) & 0x3));
		;
	}	
	
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
				scheduler_tasks[i].ready = 1;
			}
		}
	}
}
