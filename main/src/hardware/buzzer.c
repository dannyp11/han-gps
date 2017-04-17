#include <avr/io.h>
#include "buzzer.h"

#define BUZZ_off()  BUZZ_PORT&=~_BV(BUZZ_PIN)
#define BUZZ_on() BUZZ_PORT|=_BV(BUZZ_PIN)

void buzzerInit (void) {
    DDRD  |= 0b00001000; //1 = output, 0 = input
    PORTD &= 0b11110111;
}

/*
Turns on buzzer
*/
void buzzOn(void) {
	BUZZ_on();
}

/*
Turns off buzzer
*/
void buzzOff(void) {
	BUZZ_off();
}
