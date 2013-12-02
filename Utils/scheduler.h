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


// Init scheduler.
// param display - scheduler can by default manage 8-panel display. This is the table with characters to be displayed.
// 			If 0, then displayer is not managed by scheduler.
//			Displayer have to be connected to PORTA, displayer selector have to be connected to 4 lest-meaning pins of PORTB
extern void scheduler_init(uint8_t* display);

// Start the scheduler
extern void scheduler_run();

// Add periodic task to scheduler
// param task - pointer to task function
// param interval - time in milliseconds
extern void scheduler_addPeriodicTask(TASK_PTR task, uint16_t interval);

// Add one shot task to scheduler
// param task - pointer to task function
// param interval - time in milliseconds
extern void scheduler_addOneShotTask(TASK_PTR task, uint16_t interval);



extern Task scheduler_tasks[MAX_TASK];
extern uint8_t scheduler_taskCount;
extern uint8_t scheduler_displayCount;
extern uint8_t* scheduler_display;



#endif /* SCHEDULER_H_ */
