#include "clock.h"
#include "display.h"

// **************** utils ****************
// converts pressed button to number value
// return 100 if pressed button is not number button
uint8_t buttonToNumber(uint8_t button) {
	switch(button)
	{
		case BT_0 : return 0;
		case BT_1 : return 1;
		case BT_2 : return 2;
		case BT_3 : return 3;
		case BT_4 : return 4;
		case BT_5 : return 5;
		case BT_6 : return 6;
		case BT_7 : return 7;
		case BT_8 : return 8;
		case BT_9 : return 9;
	}
	
	return 100;
}


// **************** clock ****************
SClock::SClock()
{
	_hh = 12;
	_mm = 0;
	_ss = 0;
	
	_edited = 0;
	_show = 0;
	
	for(uint8_t i = 0; i<4; ++i)
		_alarms[i].setAlarmNo(i);
	
	redraw();
}

void SClock::doAction()
{
	addSecond();
	redraw();
}

void SClock::addSecond()
{
	++_ss;
	if(_ss<60) return;
	
	_ss = 0;
	++_mm;
	if(_mm<60) 
	{
		checkAlarms();
		return;
	}
	
	_mm = 0;
	++_hh;
	if(_hh) 
	{
		checkAlarms();
		return;
	}
	
	checkAlarms();
	_hh = 0;
}

void SClock::setTime(uint8_t h, uint8_t m)
{
	if(h<24 && m<60)
	{
		_hh = h;
		_mm = m;
	}
	
	checkAlarms();
	
	_edited = 0;
	redraw();
}

void SClock::checkAlarms()
{
	// check alarms
	for(uint8_t i = 0; i<4; ++i)
		_alarms[i].tryActivate(_hh, _mm);
}

uint8_t* SClock::getDisplay()
{
	return _display;
}

void SClock::handle(uint8_t button)
{
	if(button == BT_C)
	{
		//stop alarm
		for(uint8_t i = 0; i<4; ++i) 
			_alarms[i].stop();
		return;
	}
	
	if(!_edited)
	{
		if(button == BT_A)
		{
			// change show method
			++_show;
			if(_show > 2) _show = 0;
		}
		
		if(button == BT_B)
		{
			// start edit mode
			_edited = 1;
			_editHH = _hh;
			_editMM = _mm;
		}
		
		redraw();
		return;
	}
	
	// editing
	if(button == BT_B)	// cancel
	{
		_edited = 0;
		
		redraw();
		return;
	}
	
	uint8_t pressed = buttonToNumber(button);
	if(pressed>9) return;
	
	switch(_edited)
	{
		case 1 : 
			_editHH = pressed * 10;
			break;
				
		case 2 :
			_editHH += pressed;
			break;
				
		case 3: 
			_editMM = pressed * 10;
			break;
				
		case 4:
			_editMM += pressed;
			break;
	}
	
	++_edited;
	if(_edited > 4) setTime(_editHH, _editMM);
	redraw();
}

void SClock::redraw()
{
	if(!_edited)
	{
		uint8_t f = _hh;
		uint8_t s = _mm;
		
		switch(_show)
		{
			case 1 :	// 12h
				if(f>12) f -= 12;
				break;
				
			case 2:		// MM:SS
				f = _mm;
				s = _ss;
				break;
				
		}
		
		if(f>9)
			binToDecDigits(f*100 + s, _display, 4);
		else
			binToDecDigits(f*100 + s, _display, 3);	// don't show first digit
			
		return;
	} 
	
	// draw during edition time
	binToDecDigits(_editHH*100 + _editMM, _display, 4);	// show first digit, so we see what we are editing
	
	// blinking every second
	if(_ss & 1)
		_display[4 - _edited] = 0xFF;	// first we are editing digit most on the left, which is last in _display array
}

void SClock::clearAlarm(uint8_t noAlarm)
{
	if(noAlarm<4)
		_alarms[noAlarm].setEnabled(false);
}

void SClock::setAlarm(uint8_t noAlarm, uint8_t h, uint8_t m)
{
	if(noAlarm<4)
		_alarms[noAlarm].setAlarm(h, m);
}


// **************** alarm ****************
SAlarm::SAlarm()
{
	_isEnabled = false;
	_hh = 12;
	_mm = 0;
}

void SAlarm::setEnabled(bool enabled)
{
	_isEnabled = enabled;
}

void SAlarm::tryActivate(uint8_t h, uint8_t m)
{
	if(m==_mm && h==_hh) {
		PINB |= (1<<(4+no));
	}
}

void SAlarm::setAlarm(uint8_t h, uint8_t m)
{
	if(h<24 && m<60)
	{
		_hh = h;
		_mm = m;
		_isEnabled = true;
	}
}

void SAlarm::stop() 
{
	PINB &= ~(1<<(4+no));
}

void SAlarm::setAlarmNo(uint8_t no)
{
	_alarmNo = no;
}