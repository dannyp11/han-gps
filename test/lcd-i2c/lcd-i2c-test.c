/*
 * Demo LCD I2C connection
 */

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>

#include "LCD.h"

/* Test strings to store in the LCD */
char *strs = "Hello world";

#define BUFF_LEN LCD_LINE_LEN + 1

int main(void)
{
	char buffer[BUFF_LEN];

	LCDInit();
	LCDSendCommand(CURSOROFF);
	LCDSendCommand(SHOWFIRMWARE);

	LCDSetCursor(2, 0);
	LCDPrint("LCD Demo by Dat");
	LCDSetCursor(3, 0);
	snprintf(buffer, BUFF_LEN, "Version: %s", __DATE__);
	LCDPrint(buffer);

	char i;
	for (i = 0; i < 5; ++i)
	{
		LCDSetCursor(4, 0);
		snprintf(buffer, BUFF_LEN, "Demo in %d seconds...", 5 - i);
		LCDPrint(buffer);
		_delay_ms(1000);
	}

	/*
	 * brightness
	 */
	LCDSendCommand(CLEARSCREEN);
	LCDPrint("Changing brightness");
	for (i = 1; i <= 8; ++i)
	{
		LCDSetCursor(3, 0);
		snprintf(buffer, BUFF_LEN, "Level: %d", (int) i);
		LCDPrint(buffer);
		LCDSetBrightness(i);
		_delay_ms(500);
	}
	LCDInit();

	/*
	 * contrast
	 */
	LCDPrint("Changing contrast");
	for (i = 1; i <= 50; ++i)
	{
		LCDSetCursor(3, 0);
		snprintf(buffer, BUFF_LEN, "Level: %d", (int) i);
		LCDPrint(buffer);
		LCDSetContrast(i);
		_delay_ms(50);
	}
	LCDInit();

	/*
	 * Turn on and off
	 */
	LCDPrint("   Blinking screen");
	for (i = 1; i <= 5; ++i)
	{
		LCDSendCommand(LCDOFF);
		_delay_ms(500);
		LCDSendCommand(LCDON);
		_delay_ms(500);
	}
	LCDInit();

	LCDSetCursor(2, 5);
	LCDPrint("Done demo");

	while (1)
	{                 // Loop forever
	}

	return 0;
}

