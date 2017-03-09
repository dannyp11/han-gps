/********************************************
 *
 *  Name: Dat Pham
 *  Section: Wed 2pm
 *  Assignment: Lab 9b
 *
 ********************************************/

#include <stdio.h>
#include <string.h>

#include "../lcd-i2c/LCD.h"
#include "RotaryEncoder.h"

char msg[21];
char isCleaned = 0;

void printRotaryVal(uint16_t val)
{
	if (!isCleaned)
	{
		LCDSendCommand(CLEARSCREEN);
		isCleaned = 1;
	}

	LCDSendCommand(CURSORHOME);
	snprintf(msg, 21, "Value: %d", val);
	LCDPrint(msg);
}

int main(void)
{
	LCDInit();
	RotaryEncoderInit();
	LCDPrint("Rotary Encoder Demo");
	LCDSetCursor(2, 0);
	LCDPrint("Author: Dat Pham");
	LCDSetCursor(3, 0);
	snprintf(msg, 21, "Version: %s", __DATE__);
	LCDPrint(msg);
	LCDSetCursor(4, 0);
	LCDPrint("Rotate it to demo");

	/*
	 * Whenever the encoder gets new value, it will call printRotaryVal
	 */
	RotaryEncoderSetCallback(printRotaryVal);

	while (1)
	{
	};
	return 0; /* never reached */
}
