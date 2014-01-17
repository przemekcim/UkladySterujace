#ifndef SERIAL_H_
#define SERIAL_H_

#include <avr/io.h>
#include <avr/interrupt.h>

extern void serial_init(void (*inCallback)(char*));
extern int serial_send(const char* val);
extern uint8_t getSize(const char* msg);

#endif /* SERIAL_H_ */