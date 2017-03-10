/*
 * RotaryEncoder.c
 *
 *  Created on: Mar 2, 2017
 *      Author: dat
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "RotaryEncoder.h"

volatile char old_raw_input;
RotaryEncoderChangeCallback mFunction = 0;
char mSensitivityCount = 0;

void RotaryEncoderInit()
{
	PORTC |= (1 << PC2) | (1 << PC3);
	DDRC &= ~(1 << PC2) & ~(1 << PC3);

	// interrupt enable for A2 and A3 pins
	PCICR |= (1 << PCIE1);
	PCMSK1 |= (1 << PCINT10) | (1 << PCINT11);
	///////////////////////////////////////

	old_raw_input = (PINC & (1 << PC2 | 1 << PC3)) >> PC2;

	// global interrupt
	sei();
}

// decode using state diagram, output would be +1 (CW) or -1 (CCW) or 0
char decoded_value(char raw_value, char old_value)
{
	char result = 0;
	switch (old_value)
	{
	case 0b00:
		if (raw_value == 0b01)
			result = 1;
		else if (raw_value == 0b10)
			result = -1;
		break;

	case 0b01:
		if (raw_value == 0b11)
			result = 1;
		else if (raw_value == 0b00)
			result = -1;
		break;

	case 0b11:
		if (raw_value == 0b10)
			result = 1;
		else if (raw_value == 0b01)
			result = -1;
		break;

	case 0b10:
		if (raw_value == 0b00)
			result = 1;
		else if (raw_value == 0b11)
			result = -1;
		break;
	}
	return result;
}

ISR(PCINT1_vect)
{
	char raw_input = (PINC & (1 << PC2 | 1 << PC3)) >> PC2; // get raw_input = 0xAB
	if (mFunction)
	{
		char decoded_val = decoded_value(raw_input, old_raw_input);
		if (decoded_val)
		{
			mFunction(decoded_val);
		}
	}

	old_raw_input = raw_input;
}

uint8_t RotaryEncoderSetCallback(RotaryEncoderChangeCallback function)
{
	if (mFunction != 0)
	{
		return -1;
	}
	else
	{
		mFunction = function;
	}
	return 0;
}
