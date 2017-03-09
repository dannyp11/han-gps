#include <avr/io.h>
#include <util/delay.h>

#include "LED.h"


int main(void) {
	
	ioinit(); //Setup IO pins and defaults

	while (1) {
			LEDall();
      _delay_ms(300);
     LEDclear();
      _delay_ms(300);
      LEDall();
      _delay_ms(300);
      LEDclear(); 
      _delay_ms(300);
      LEDnorth();
      _delay_ms(300);
      LEDnortheast();
      _delay_ms(300);
      LEDeast();
      _delay_ms(300);
      LEDsoutheast();
      _delay_ms(300);
      LEDsouth();
      _delay_ms(300);
      LEDsouthwest();
      _delay_ms(300);
      LEDwest();
      _delay_ms(300);
      LEDnorthwest();
      _delay_ms(300);
	}

	return 0; /* never reached */
}

