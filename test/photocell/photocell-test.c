#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <string.h>

#include "../lcd-i2c/LCD.h"
#include "Photocell.h"

char msg[20];
printPhotocell(uint16_t val)
{
	LCDSendCommand(CURSORHOME);
	snprintf(msg, 20, "photo = %u", val);
	LCDPrint(msg);

	if (val != 10)
	{
		LCDSetBrightness(val);
	}
}

int main(void)
{
	LCDInit();
	PhotocellInit();

	PhotocellRegisterCallback(printPhotocell);

	while (1)
	{
	};
	return 0; /* never reached */
}
