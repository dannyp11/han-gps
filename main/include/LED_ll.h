/*
 * LED.h
 */

#ifndef LED_H_
#define LED_H_

#include <hal.h>

#define SER_PORT    IOPORT4
#define SER_PIN     5
#define RCLK_PORT IOPORT4
#define RCLK_PIN  6
#define SRCLK_PORT IOPORT4
#define SRCLK_PIN  4
 
// #define SER_low()  SER_PORT&=~_BV(SER_PIN)
// #define SER_high() SER_PORT|=_BV(SER_PIN)
// #define RCLK_low()  RCLK_PORT&=~_BV(RCLK_PIN)
// #define RCLK_high() RCLK_PORT|=_BV(RCLK_PIN)
// #define SRCLK_low()  SRCLK_PORT&=~_BV(SRCLK_PIN)
// #define SRCLK_high() SRCLK_PORT|=_BV(SRCLK_PIN)

//Define functions
//===============================================

/*
Sets ATmega328P ports PD2, PD3, PD4 to output and
initializes all LEDs to OFF
*/
void LEDinit(void);

/*
Sends '__led_state' pattern to LEDs
*/
void output_led_state(unsigned int __led_state);

/*
Turns on all the LEDs
*/
void LEDall(void);

/*
Turns off all the LEDs
*/
void LEDclear(void);

/*
Turns on the NORTH LED.
It stays on until another LED is turned on or they are
all cleared.
*/
void LEDnorth(void);

/*
Turns on the EAST LED.
It stays on until another LED is turned on or they are
all cleared.
*/
void LEDeast(void);

/*
Turns on the SOUTH LED.
It stays on until another LED is turned on or they are
all cleared.
*/
void LEDsouth(void);

/*
Turns on the WEST LED.
It stays on until another LED is turned on or they are
all cleared.
*/
void LEDwest(void);

/*
Turns on the NORTHEAST LED.
It stays on until another LED is turned on or they are
all cleared.
*/
void LEDnortheast(void);

/*
Turns on the SOUTHEAST LED.
It stays on until another LED is turned on or they are
all cleared.
*/
void LEDsoutheast(void);

/*
Turns on the NORTHWEST LED.
It stays on until another LED is turned on or they are
all cleared.
*/
void LEDnorthwest(void);

/*
Turns on the SOUTHWEST LED.
It stays on until another LED is turned on or they are
all cleared.
*/
void LEDsouthwest(void);

//===============================================


 #endif /* LED_H_ */