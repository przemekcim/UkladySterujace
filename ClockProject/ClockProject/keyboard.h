#ifndef KEYBOARD_H_
#define KEYBOARD_H_

#include <avr/io.h>

// reads input from keyboard
// uses PORTC
extern uint8_t readKeyboard();

// Gets value returned by readKeyboard() and returns value 0-16
// 0 : no key pressed. 1-16 : id of key pressed
extern uint8_t keyboardToInt(uint8_t val);



#endif 