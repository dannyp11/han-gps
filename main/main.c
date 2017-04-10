// chibios includes here
#include "ch.h"
#include "chprintf.h"
#include "hal.h"

// HW includes (avr, lcd, gps, etc.) here
#include <avr/io.h>
#include "LCD.h"
#include "LED.h"

// SW includes (modules, algorithm, etc.) here
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

/*
 * ChibiOS config
 */
#define DRIVERPRIO HIGHPRIO

/*
 * Global variables here, pls don't use static
 */
char msg[20];
int i;

/*
 * Other helper functions
 * Pls create module if these functions are too big, for structure control purpose
 */
#define LCD_WA_SIZE 128
#define BUFF_LEN LCD_LINE_LEN + 1
void lcd_demo(void)
{
	static char buffer[BUFF_LEN];

	LCDSendCommand(CURSOROFF);
	LCDSendCommand(SHOWFIRMWARE);

	LCDSetCursor(2, 0);
	LCDPrint("LCD:D.Pham,OS:R.Dong");
	LCDSetCursor(3, 0);
	chsnprintf(buffer, BUFF_LEN, "Version: %s", __DATE__);
	LCDPrint(buffer);

	char i;
	for (i = 0; i < 5; ++i)
	{
		LCDSetCursor(4, 0);
		chsnprintf(buffer, BUFF_LEN, "Demo in %d seconds...", 5 - i);
		LCDPrint(buffer);
		chThdSleepMilliseconds(1000);
	}
	LCDReset();

	/*
	 * brightness
	 */
	LCDPrint("Changing brightness");
	for (i = 1; i <= 8; ++i)
	{
		LCDSetCursor(3, 0);
		chsnprintf(buffer, BUFF_LEN, "Level: %d", (int) i);
		LCDPrint(buffer);
		LCDSetBrightness(i);
		chThdSleepMilliseconds(500);
	}
	LCDReset();

	/*
	 * contrast
	 */
	LCDPrint("Changing contrast");
	for (i = 1; i <= 50; ++i)
	{
		LCDSetCursor(3, 0);
		chsnprintf(buffer, BUFF_LEN, "Level: %d", (int) i);
		LCDPrint(buffer);
		LCDSetContrast(i);
		chThdSleepMilliseconds(50);
	}
	LCDReset();

	/*
	 * Turn on and off
	 */
	char *blinking_msg = "..Blinking Srceen...";
	LCDPrint(blinking_msg);
	LCDSetCursor(2, 0);
	LCDPrint(blinking_msg);
	LCDSetCursor(3, 0);
	LCDPrint(blinking_msg);
	LCDSetCursor(4, 0);
	LCDPrint(blinking_msg);
	for (i = 1; i <= 5; ++i)
	{
		LCDSendCommand(LCDOFF);
		chThdSleepMilliseconds(500);
		LCDSendCommand(LCDON);
		chThdSleepMilliseconds(500);
	}
	LCDReset();

	/*
	 * Move cursor
	 */
	LCDPrint("Move Cursor");
	chThdSleepMilliseconds(2000);
	LCDSendCommand(CLEARSCREEN);
	LCDSendCommand(CURSORON);
	char j;
	for (i = 1; i <= 4; ++i)
	{
		for (j = 0; j < 20; ++j)
		{
			LCDSetCursor(i, j);
			chThdSleepMilliseconds(100);
		}
	}
	LCDReset();

	LCDSetCursor(2, 5);
	LCDPrint("Done demo");
}

THD_WORKING_AREA(waTdLCD, LCD_WA_SIZE);
THD_FUNCTION(tdLCD, arg)
{
	(void) arg;
	while (true)
	{
		snprintf(msg, 20, "i = %d", i);
		LCDSetCursor(2,0);
		LCDPrint(msg);
	}
}

/*
 * Application entry point.
 */
int main(void)
{
	/*
	 * System initializations.
	 * - HAL initialization, this also initializes the configured device drivers
	 *   and performs the board-specific initializations.
	 * - Kernel initialization, the main() function becomes a thread and the
	 *   RTOS is active.
	 */
	halInit();
	chSysInit();

	/*
	 * Init all modules here
	 * initialization shouldn't go into thread since it's only called once
	 */
	LCDInit();
	LEDinit();

	/*
	 * Inits all global variables here
	 */
	i = 0;
	snprintf(msg, 20, "hi");
	LCDPrint(msg);

	/*
	 * Run all threads
	 */
	chThdCreateStatic(waTdLCD, sizeof(waTdLCD), DRIVERPRIO, tdLCD, NULL);

	chThdSleepSeconds(1);
	while (true)
	{
		++i;

		LEDall();
		chThdSleepMilliseconds(500);
		LEDclear();
		chThdSleepMilliseconds(500);
	}
}
