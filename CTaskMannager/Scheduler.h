#ifndef SCHEDULER_H_
#define SCHEDULER_H_

#include <avr/io.h>
#include <avr/interrupt.h>

#define NON_PERIODIC 0
#define MAX_TASK 10

typedef void (*TASK_PTR) (void);

typedef struct
{
	TASK_PTR task;
	uint8_t ready;
	uint16_t toGo;
	uint16_t interval;
} Task;


extern void scheduler_init(uint8_t* display);

extern void scheduler_run();
extern void scheduler_addPeriodicTask(TASK_PTR task, uint16_t interval);
extern void scheduler_addOneShotTask(TASK_PTR task, uint16_t interval);

extern Task scheduler_tasks[MAX_TASK];
extern uint8_t scheduler_taskCount;
extern uint8_t scheduler_displayCount;
extern uint8_t* scheduler_display;



#endif /* SCHEDULER_H_ */
