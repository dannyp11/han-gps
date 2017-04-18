/*
 * Button.c
 *
 *  Created on: Mar 9, 2017
 *      Author: dat
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "Button.h"

static ButtonPressCallback mOkFunction = 0;
static ButtonPressCallback mCancelFunction = 0;

void ButtonsInit()
{
	//enable pullup
	PORTB |= ((1 << BUTTON_CANCEL) | (1 << BUTTON_OK));

	// set as input
	DDRB &= ~(1 << BUTTON_CANCEL);
	DDRB &= ~(1 << BUTTON_OK);

	// enable interrupt
	PCICR |= (1 << PCIE0);
	PCMSK0 |= (1 << BUTTON_CANCEL) | (1 <<BUTTON_OK);

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
