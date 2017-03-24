/*
 * Demo Compass I2C connection
 */

//#include "../lcd-i2c/LCD.h"

#include "Compass.h"

//#define BUFF_LEN (LCD_LINE_LEN + 1)

int main(void)
{
//	SerialDebugInit();
//	LCDInit();
//
//	LCDPrint("Testing Compass");

	CompassInit();

	while (1)
	{                 // Loop forever
	}

	return 0;
}

