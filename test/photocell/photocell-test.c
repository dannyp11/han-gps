#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <string.h>
#include <util/delay.h>

#include "../lcd-i2c/LCD.h"
#include "Photocell.h"
#include "../led/LED.h"

char msg[21];
printPhotocell(uint8_t val)
{
	LCDSendCommand(CURSORHOME);
	snprintf(msg, 21, "Value = %u", val);
	LCDPrint(msg);

	if (val != 10)
	{
		LCDSetBrightness((char)val);
	}
}

int main(void)
{
	LCDInit();
	PhotocellInit();

	snprintf(msg, 21, "Photo cell testing");
	LCDSetCursor(3, 0);
	LCDPrint(msg);

	PhotocellRegisterCallback(printPhotocell);

	while (1)
	{
	};
	return 0; /* never reached */
}
