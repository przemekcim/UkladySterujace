#ifndef DISPLAY_H_
#define DISPLAY_H_

extern const uint8_t DIGITS[16];
extern const uint8_t DIGIT_DOT;

// Change binary int to char decimal representation prepared to be displayed on 8-panel displays
// param val binary int value
// param tab array of char where result will be copied
// param n size of tab
extern void binToDecDigits(uint16_t val, uint8_t* tab, uint8_t n);

#endif /* DISPLAY_H_ */