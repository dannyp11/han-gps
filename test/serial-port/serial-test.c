/*
 * Demo Serial connection with pc
 */

#include <util/delay.h>

#include "SerialDebug.h"

char str1[] = "12345678901234567890";
char str2[] = ">> USC EE459L << ";
char str3[] = ">> at328-6.c <<< ";
char str4[] = "-- April 11, 2011 -- ";

int main(void)
{

	SerialDebugInit();

	while (1)
	{               // Loop forever
		SerialDebugPrint(str1);
		SerialDebugPrint(str3);
		SerialDebugPrint(str4);
		SerialDebugPrint(str2);
		_delay_ms(100);
	}

	return 0; /* never reached */
}
