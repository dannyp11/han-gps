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

  TOGGLE_PIN(DDRC, 0, 1); //LED output
  TOGGLE_PIN(DDRC, 1, 0); //push-button input
  TOGGLE_PIN(PORTC, 0, 0); //LED default LOW

  char *coord[3];
  coord[0] = "#01,051301211,000073945,0$";
  coord[1] = "#whats up$";
  coord[2] = "#almost done!!!$";
  int indx = 0;
  int prev = indx;

  while(1) {
    if (PINC & (1 << PC1)) {
      if (indx == prev) {
        indx = (++indx)%3;
      }
    } else {
      prev = indx;
    }
    //TOGGLE_PIN(PORTC, 0, 1);
    SerialDebugPrint(coord[indx]);
    _delay_ms(100);
    //TOGGLE_PIN(PORTC, 0, 0);
    _delay_ms(100);
  }

  return 0;

}


