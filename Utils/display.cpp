#include "display.h"

// chars which can be represented on 8-panel display ['0'-'F']
const uint8_t DIGITS[16] = {~0x3F, ~0x06, ~0x5B, ~0x4F, ~0x66, ~0x6D, ~0x7D, ~0x07, ~0x7F, ~0x6F, ~0x77, ~0x7C, ~0x39, ~0x5e, ~0x79, ~0x71};
const uint8_t DIGIT_DOT = 0x7f;

void binToDecDigits(uint16_t val, uint8_t* tab, uint8_t n)
{
	for(uint8_t i = 0; i<4; ++i) {
		if(i<n) {
			tab[i] = DIGITS[val % 10];
			val /= 10;
		} else {
			tab[i] = 0xFF;
		}		
	}
}
