#include <avr/io.h>
#include "clock.h"
#include "keyboard.h"
#include "scheduler.h"
#include "serial.h"
#include "HD44780.h"

SClock gClock;	// global clock
const char* responseMsg = 0;
const char* invalidMsg = "\nInvalid format\n>";
const char* correctMsg = "\nDone\n>";

void oneSecondTask()
{
	gClock.doAction();
}

void keyboardTask()
{
	uint8_t pressed = keyboardToInt(readKeyboard());
	if(pressed) gClock.handle(pressed);
	
	// this task also is responsible for response
	if(responseMsg) serial_send(responseMsg);
	responseMsg = 0;
}

// parses serial input message
// format:
// sc 1234 [s]ets [c]lock to [12]:[34]
// s1 1544 [s]ets alarm [1] to [15]:[44]
// c4 [c]lears alarm [4]
void onReceiveTask(char* msg)
{
	uint8_t size = getSize(msg);
	
	// alarm clearing
	if(size == 2)
	{
		uint8_t noAlarm = msg[1]-'1';
		if(msg[0] != 'c' || noAlarm>3)
		{
			responseMsg = invalidMsg;
			return;
		}
		
		// clear this clock
		gClock.clearAlarm(noAlarm);
		responseMsg = correctMsg;
		return;
	}
	
	if(size != 7 || msg[0] != 's')
	{
		responseMsg = invalidMsg;
		return;
	}
	
	uint8_t arg1 = msg[3]-'0';
	uint8_t arg2 = msg[4]-'0';
	uint8_t arg3 = msg[5]-'0';
	uint8_t arg4 = msg[6]-'0';
	uint8_t command = msg[1];
	
	if(command == 'c')
	{
		gClock.setTime(arg1*10+arg2, arg3*10+arg4);
		responseMsg = correctMsg;
	} else if(command>'0' && command<'5') {
		gClock.setAlarm(command-'1', arg1*10+arg2, arg3*10+arg4);
		responseMsg = correctMsg;
	} else {
		responseMsg = invalidMsg;
	}
}

int main(void)
{
	// scheduler init
    scheduler_init(gClock.getDisplay());
    scheduler_addPeriodicTask(oneSecondTask, 1000);
	scheduler_addPeriodicTask(keyboardTask, 200);
	
	// serial init
	serial_init(onReceiveTask);
    
	// LCD init
    LCD_Initalize();
    LCD_Command(HD44780_DISPLAY_ONOFF|HD44780_DISPLAY_ON);
    
    scheduler_run();
}