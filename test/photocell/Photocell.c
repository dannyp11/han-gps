#include <avr/io.h>
#include <avr/interrupt.h>

#include "Photocell.h"


#define CUTOFF_LIGHT_LEVEL 40000
PhotocellCallbackFunction mFunction = 0;
uint8_t _old_val = 10;

void PhotocellInit()
{
	ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // Set ADC prescaler to 128 - 125KHz sample rate @ 16MHz
	ADMUX |= (1 << REFS0); // Set ADC reference to AVCC
	ADMUX |= (1 << ADLAR); // Left adjust ADC result to allow easy 8 bit reading
	ADMUX |= (1 << MUX0);
	// No MUX values needed to be changed to use ADC0
	ADCSRA |= (1 << ADATE);  // Set ADC to Free-Running Mode
	ADCSRA |= (1 << ADEN);  // Enable ADC
	ADCSRA |= (1 << ADIE);  // Enable ADC Interrupt
	sei();
	// Enable Global Interrupts
	ADCSRA |= (1 << ADSC);  // Start A2D Conversions
	sei();
}

uint8_t PhotocellRegisterCallback(PhotocellCallbackFunction function)
{
	if (mFunction != 0)
	{
		return 1;
	}
	else
	{
		mFunction = function;
	}
	return 0;
}

ISR(ADC_vect)
{
	if (mFunction)
	{
		uint8_t val = PhotocellGetAbienceLightLevel();
		if (_old_val != val)
		{
			mFunction(val);
			_old_val = val;
		}
	}
}

uint8_t PhotocellGetAbienceLightLevel()
{
	uint8_t retVal = 10;

	if (ADCW < (uint16_t)CUTOFF_LIGHT_LEVEL)
	{
		retVal = 0;
	}
	else
	{
		retVal = (uint8_t)( (ADCW - CUTOFF_LIGHT_LEVEL) / ((65535 - CUTOFF_LIGHT_LEVEL) / 9) );
	}

	return retVal;
}
