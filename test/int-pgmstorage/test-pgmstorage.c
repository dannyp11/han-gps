#include <util/delay.h>
#include <stdio.h>

#include "../lcd-i2c/LCD.h"
#include "../serial-port/SerialDebug.h"
#include "PgmStorage.h"

const char PROGMEM mem1[] = "hi";
const char PROGMEM mem2[] = "hey";
const char PROGMEM mem3[] = "haha";

int main()
{
	LCDInit();
	SerialDebugInit();

	char msg[20];
	snprintf(msg, 20, "Testing pgmspace        ");
	LCDSetCursor(1, 0);
	LCDPrint(msg);

	while (1)
	{
		PgmStorageGet(msg, mem1);
		LCDSetCursor(3, 0);
		LCDPrint(msg);

		_delay_ms(200);
	}

	return 0;
}

