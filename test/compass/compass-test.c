/*
 * Demo Compass I2C connection
 */

#include <util/delay.h>
#include <stdio.h>
#include <string.h>

#include "Compass.h"
#include "../lcd-i2c/LCD.h"
#include "../serial-port/SerialDebug.h"
#include "../led/LED.h"

#define BUFF_LEN LCD_LINE_LEN + 1

int main(void)
{
	ioinit();
	LEDclear();
	LCDInit();
	SerialDebugInit();
	LCDPrint("Testing Compass");

	CompassInit();
	char msgLCD[BUFF_LEN];
	char msgSerial[80];
	char directionText[10];

	while (1)
	{
		// Loop forever
		float header = CompassGetAngle();
		CompassDirection direction = CompassGetDirection();

		LCDSetCursor(3, 0);
		snprintf(msgLCD, 20, "header angle %3.0f", header);
		LCDPrint(msgLCD);
		LCDSetCursor(4, 0);
		LCDPrint("                    ");
		LCDSetCursor(4, 0);
		LCDPrint(directionText);

		CompassGetDirectionText(directionText, direction);
		snprintf(msgSerial, 80, "compass angle %3.3f direction %s", header, directionText);
		SerialDebugPrint(msgSerial);

		switch (direction)
		{
		case NORTH:
		{
			LEDnorth();
		}
			break;
		case EAST:
		{
			LEDeast();
		}
			break;
		case SOUTH:
		{
			LEDsouth();
		}
			break;
		case WEST:
		{
			LEDwest();
		}
			break;
		case NORTHEAST:
		{
			LEDnortheast();
		}
			break;
		case NORTHWEST:
		{
			LEDnorthwest();
		}
			break;
		case SOUTHEAST:
		{
			LEDsoutheast();
		}
			break;
		case SOUTHWEST:
		{
			LEDsouthwest();
		}
			break;
		default:
			break;
		}

		_delay_ms(200);
	}

	return 0;
}

