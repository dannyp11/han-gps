#include "led.h"

/*
initializes all LEDs to OFF
*/
extern void LEDinit(void);

/*
Sends '__led_state' pattern to LEDs
*/
extern void output_led_state(unsigned int __led_state);

/*
Turns on all the LEDs
*/
extern void LEDall(void);

/*
Turns off all the LEDs
*/
extern void LEDclear(void);

/*
Turns on the NORTH LED.
It stays on until another LED is turned on or they are
all cleared.
*/
extern void LEDnorth(void);

/*
Turns on the EAST LED.
It stays on until another LED is turned on or they are
all cleared.
*/
extern void LEDeast(void);

/*
Turns on the SOUTH LED.
It stays on until another LED is turned on or they are
all cleared.
*/
extern void LEDsouth(void);

/*
Turns on the WEST LED.
It stays on until another LED is turned on or they are
all cleared.
*/
extern void LEDwest(void);

/*
Turns on the NORTHEAST LED.
It stays on until another LED is turned on or they are
all cleared.
*/
extern void LEDnortheast(void);

/*
Turns on the SOUTHEAST LED.
It stays on until another LED is turned on or they are
all cleared.
*/
extern void LEDsoutheast(void);

/*
Turns on the NORTHWEST LED.
It stays on until another LED is turned on or they are
all cleared.
*/
extern void LEDnorthwest(void);

/*
Turns on the SOUTHWEST LED.
It stays on until another LED is turned on or they are
all cleared.
*/
extern void LEDsouthwest(void);

void led_demo(void) {

  while (1) {
    LEDall();
    chThdSleepMilliseconds(300);
    LEDclear();
    chThdSleepMilliseconds(300);
    LEDall();
    chThdSleepMilliseconds(300);
    LEDclear();
    chThdSleepMilliseconds(300);
    LEDall();
    chThdSleepMilliseconds(300);
    LEDclear();
    chThdSleepMilliseconds(300);
    LEDnorth();
    chThdSleepMilliseconds(300);
    LEDnortheast();
    chThdSleepMilliseconds(300);
    LEDeast();
    chThdSleepMilliseconds(300);
    LEDsoutheast();
    chThdSleepMilliseconds(300);
    LEDsouth();
    chThdSleepMilliseconds(300);
    LEDsouthwest();
    chThdSleepMilliseconds(300);
    LEDwest();
    chThdSleepMilliseconds(300);
    LEDnorthwest();
    chThdSleepMilliseconds(300);
  }
}

#define LED_WA_SIZE 128

THD_WORKING_AREA(waTdLED, LED_WA_SIZE);
THD_FUNCTION(tdLED, arg) {
  (void)arg;
  LEDinit(); //Setup IO pins and defaults
  while (true) {
    led_demo();
    chThdSleepSeconds(1);
  }
}