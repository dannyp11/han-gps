#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>

#include "../../serial-port/SerialDebug.h"

#define TOGGLE_PIN(port, pin, isOn) \
{ if (isOn == 0) port &= ~(1 << pin); \
  else port |= (1 << pin); \
}

int main(void) {

  SerialDebugInit();

  //TOGGLE_PIN(DDRC, 0, 1); //LED output
  //TOGGLE_PIN(PORTC, 0, 0); //LED default LOW

  char str[] = "_lat=1234556";


  while(1) {
    //TOGGLE_PIN(PORTC, 0, 1);
    SerialDebugPrint(str);
    _delay_ms(100);
    //TOGGLE_PIN(PORTC, 0, 0);
    _delay_ms(100);
  }

  return 0;

}


