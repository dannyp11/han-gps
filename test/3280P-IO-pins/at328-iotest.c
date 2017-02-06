/*************************************************************
 *       Demonstrate simple I/O pin test of ATmega328
 *	Author: Dat
 *
 * 	This program tests 20 IO pins: PB0-PB5, PC0-PC5, PD0-PD7
 *
 * Code structure:
 * 	Set all I / O pins to zero
 *	Set all I / O pins to be outputs
 *	Start of loop
 *		Trigger pin = 1
 *		Trigger pin = 0
 *		I / O pin #2 = 1
 *		I / O pin #2 = 0
 *		I / O pin #3 = 1
 *		I / O pin #3 = 0
 *		.
 *		.
 *		.
 *		I / O pin # n = 1
 *		I / O pin # n = 0
 *	End of loop
 *
 *************************************************************/

#include <avr/io.h>
#include <util/delay.h>

#define TOGGLE_PIN(port, pin, isOn) \
{	if (isOn == 0) port &= ~(1 << pin); \
	else port |= (1 << pin); \
}

int main(void) {
	// Set all pins to output
	unsigned char i;
	for (i = 0; i < 8; ++i) {
		TOGGLE_PIN(DDRD, i, 1);
	}
	for (i = 0; i < 6; ++i) {
		TOGGLE_PIN(DDRB, i, 1);
		TOGGLE_PIN(DDRC, i, 1);
	}

	//Set all pins to 0
	for (i = 0; i < 8; ++i) {
		TOGGLE_PIN(PORTD, i, 0);
	}
	for (i = 0; i < 6; ++i) {
		TOGGLE_PIN(PORTB, i, 0);
		TOGGLE_PIN(PORTC, i, 0);
	}

	while (1) {
		PORTD |= (1 << 0);
		PORTD &= ~(1 << 0);
		PORTD |= (1 << 1);
		PORTD &= ~(1 << 1);
		PORTD |= (1 << 2);
		PORTD &= ~(1 << 2);
		PORTD |= (1 << 3);
		PORTD &= ~(1 << 3);
		PORTD |= (1 << 4);
		PORTD &= ~(1 << 4);
		PORTD |= (1 << 5);
		PORTD &= ~(1 << 5);
		PORTD |= (1 << 6);
		PORTD &= ~(1 << 6);
		PORTD |= (1 << 7);
		PORTD &= ~(1 << 7);

		PORTB |= (1 << 0);
		PORTB &= ~(1 << 0);
		PORTB |= (1 << 1);
		PORTB &= ~(1 << 1);
		PORTB |= (1 << 2);
		PORTB &= ~(1 << 2);
		PORTB |= (1 << 3);
		PORTB &= ~(1 << 3);
		PORTB |= (1 << 4);
		PORTB &= ~(1 << 4);
		PORTB |= (1 << 5);
		PORTB &= ~(1 << 5);

		PORTC |= (1 << 0);
		PORTC &= ~(1 << 0);
		PORTC |= (1 << 1);
		PORTC &= ~(1 << 1);
		PORTC |= (1 << 2);
		PORTC &= ~(1 << 2);
		PORTC |= (1 << 3);
		PORTC &= ~(1 << 3);
		PORTC |= (1 << 4);
		PORTC &= ~(1 << 4);
		PORTC |= (1 << 5);
		PORTC &= ~(1 << 5);
	}

	return 0; /* never reached */
}
