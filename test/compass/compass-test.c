/*
 * Demo Compass I2C connection
 */

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>

#include "../lcd-i2c/LCD.h"

#define BUFF_LEN LCD_LINE_LEN + 1

int main(void)
{
	char buffer[BUFF_LEN];

	LCDInit();
	LCDPrint("Testing Compass");

	CompassInit();

	while (1)
	{                 // Loop forever
	}

	return 0;
}

