#include "Scheduler.h"

void  task(void)
{
	PORTA ^= 1;
}

void  task2(void)
{
	PORTA ^= 2;
}

void  task3(void)
{
	PORTA ^= 4;
}

void  task4(void)
{
	PORTA ^= 8;
}

int main(void)
{
	DDRA = 0xff;
	
	scheduler_init();
	scheduler_addPeriodicTask(task,1);
	scheduler_addPeriodicTask(task2,2);	
	scheduler_addPeriodicTask(task3,1);
	scheduler_addPeriodicTask(task4,2);
	
    scheduler_run();
}
