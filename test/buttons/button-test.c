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
#include "Button.h"

char msg[21];
char isCleaned = 0;

void callbackCancel()
{
	if (!isCleaned)
	{
		LCDSendCommand(CLEARSCREEN);
		isCleaned = 1;
	}

	LCDSendCommand(CURSORHOME);
	LCDSendCommand(CLEARSCREEN);
	LCDPrint("Cancel pressed");
}

void callbackOk()
{
	if (!isCleaned)
	{
		LCDSendCommand(CLEARSCREEN);
		isCleaned = 1;
	}

	LCDSendCommand(CURSORHOME);
	LCDSendCommand(CLEARSCREEN);
	LCDPrint("Ok pressed");
}

void callbackUp()
{
	if (!isCleaned)
	{
		LCDSendCommand(CLEARSCREEN);
		isCleaned = 1;
	}

	LCDSendCommand(CURSORHOME);
	LCDSendCommand(CLEARSCREEN);
	LCDPrint("Up pressed");
}

void callbackDown()
{
	if (!isCleaned)
	{
		LCDSendCommand(CLEARSCREEN);
		isCleaned = 1;
	}

	LCDSendCommand(CURSORHOME);
	LCDSendCommand(CLEARSCREEN);
	LCDPrint("Down pressed");
}

int main(void)
{
	LCDInit();
	ButtonsInit();

	ButtonOKSetCallback(callbackOk);
	ButtonCancelSetCallback(callbackCancel);
	ButtonUpSetCallback(callbackUp);
	ButtonDownSetCallback(callbackDown);

	LCDPrint("Button Demo");
	LCDSetCursor(2, 0);
	LCDPrint("Author: Dat Pham");
	LCDSetCursor(3, 0);
	snprintf(msg, 21, "Version: %s", __DATE__);
	LCDPrint(msg);
	LCDSetCursor(4, 0);
	LCDPrint("Press to demo");

	while (1)
	{
	};
	return 0; /* never reached */
}
