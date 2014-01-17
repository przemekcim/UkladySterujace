#ifndef CLOCK_H_
#define CLOCK_H_

#include <stdint.h>

// Buttons definition for:
// 7 8 9 A
// 4 5 6 B
// 1 2 3 C
// L 0 P D
#define BT_0 14
#define BT_1 9
#define BT_2 10
#define BT_3 11
#define BT_4 5
#define BT_5 6
#define BT_6 7
#define BT_7 1
#define BT_8 2
#define BT_9 3

#define BT_A 4
#define BT_B 8
#define BT_C 12
#define BT_D 16

#define BT_L 13
#define BT_R 15

struct SAlarm
{
	SAlarm();
	
	void setEnabled(bool enabled);			// enable / disable alarm
	void tryActivate(uint8_t h, uint8_t m);	// activates alarm if passed time is this alarm time
	void setAlarm(uint8_t h, uint8_t m);	// sets alarm
	
	uint8_t _hh;							// hours
	uint8_t _mm;							// minutes
	bool _isEnabled;						// if alarm is active
};

struct SClock 
{
	SClock();
	
	void doAction();					// action done every second
	void redraw();						// redraws clock
	void setTime(uint8_t h, uint8_t m);	// sets time
	void handle(uint8_t button);		// handles button click
	uint8_t* getDisplay();				// returns displayer array
	
	void addSecond();					// adds one second
	void checkAlarms();					// activates alarm if needed
	
	void clearAlarm(uint8_t noAlarm);	// clears alarm
	void setAlarm(uint8_t noAlarm, uint8_t h, uint8_t m);	// sets alarm
	
	uint8_t _hh;						// hours
	uint8_t _mm;						// minutes
	uint8_t _ss;						// seconds
	
	uint8_t _editHH;					// hours var used during time changing
	uint8_t _editMM;					// minutes var used during time changing
	
	uint8_t _display[4];				// displayer
	uint8_t _edited;					// which digit is currently being edited. 0 if none
	uint8_t _show;						// show option 0: 24h, 1: 12h, 2: MM:SS 
	
	SAlarm _alarms[4];					// 4 alarms
};



#endif /* CLOCK_H_ */