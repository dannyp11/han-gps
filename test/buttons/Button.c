/*
 * Button.c
 *
 *  Created on: Mar 9, 2017
 *      Author: dat
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "Button.h"

ButtonPressCallback mOkFunction = 0;
ButtonPressCallback mCancelFunction = 0;
ButtonPressCallback mUpFunction = 0;
ButtonPressCallback mDownFunction = 0;

void ButtonsInit()
{
	//enable pullup
	PORTB |= ((1 << BUTTON_CANCEL) | (1 << BUTTON_OK) | (1 << BUTTON_DOWN));
	PORTD |= (1 << BUTTON_UP);

	// set as input
	DDRB &= ~(1 << BUTTON_CANCEL);
	DDRB &= ~(1 << BUTTON_OK);
	DDRB &= ~(1 << BUTTON_DOWN);
	DDRD &= ~(1 << BUTTON_UP);

	// enable interrupt
	PCICR |= (1 << PCIE0) | (1 << PCIE2);
	PCMSK0 |= (1 << BUTTON_CANCEL) | (1 << BUTTON_OK) | (1 << BUTTON_DOWN);
	PCMSK2 |= (1 << BUTTON_UP);

	sei();
}

ISR(PCINT0_vect)
{
	if (mOkFunction && (PINB & (1 << BUTTON_OK)))
	{
		mOkFunction();
	}
	else if (mCancelFunction && (PINB & (1 << BUTTON_CANCEL)))
	{
		mCancelFunction();
	}
	else if (mDownFunction && (PINB & (1 << BUTTON_DOWN)))
	{
		mDownFunction();
	}
}

ISR(PCINT2_vect)
{
	if (mUpFunction && (PIND & (1 << BUTTON_UP)))
	{
		mUpFunction();
	}
}

uint8_t ButtonOKSetCallback(ButtonPressCallback function)
{
	if (mOkFunction)
	{
		return 1;
	}
	else
	{
		mOkFunction = function;
	}
	return 0;
}

uint8_t ButtonCancelSetCallback(ButtonPressCallback function)
{
	if (mCancelFunction)
	{
		return 1;
	}
	else
	{
		mCancelFunction = function;
	}
	return 0;
}

uint8_t ButtonUpSetCallback(ButtonPressCallback function)
{
	if (mUpFunction)
	{
		return 1;
	}
	else
	{
		mUpFunction = function;
	}
	return 0;
}

uint8_t ButtonDownSetCallback(ButtonPressCallback function)
{
	if (mDownFunction)
	{
		return 1;
	}
	else
	{
		mDownFunction = function;
	}
	return 0;
}
