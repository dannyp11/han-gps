#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>

#include "../../lcd-i2c/LCD.h"
#include "../../serial-port/SerialDebug.h"
#include "../../led/LED.h"


#define BUFF_LEN LCD_LINE_LEN + 1

#define TOGGLE_PIN(port, pin, isOn) \
{ if (isOn == 0) port &= ~(1 << pin); \
  else port |= (1 << pin); \
}


int main(void) {

	ioinit();
	LEDclear();
	SerialDebugInit();
	
	LCDInit();
	LCDSendCommand(CURSOROFF);

	char buffer[BUFF_LEN];
	size_t bufSize = 0;

	int i = 0;
	LEDall();
	while (1) {
		char byte;
		byte = serial_in();
		if (i >= 12) {
			LCDPrint(buffer);
			_delay_ms(2000);
			LCDSendCommand(CLEARSCREEN);
			memset( buffer , 0 , sizeof(char)*BUFF_LEN ) ;
			bufSize = 0;
			i = 0;
		} else if (byte == '_') {
			i = 0;
		} else {
			buffer[bufSize++] = byte;
    		i++;
		}
	}
	

	return 0;
}
