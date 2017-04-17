#include <avr/io.h>
#include <util/delay.h>

#include "buzzer.h"

void ioinit (void) {
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