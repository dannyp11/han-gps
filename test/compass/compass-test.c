/*
 * Demo Compass I2C connection
 */

#include <util/delay.h>
#include <stdio.h>
#include <string.h>

#include "../lcd-i2c/LCD.h"
#include "../serial-port/SerialDebug.h"
#include "Compass.h"

#define BUFF_LEN (LCD_LINE_LEN + 1)

int main(void)
{
	char buffer[BUFF_LEN];
	SerialDebugInit();
	LCDInit();

	LCDPrint("Testing Compass");

	CompassInit();

	while (1)
	{                 // Loop forever
	}

	return 0;
}

