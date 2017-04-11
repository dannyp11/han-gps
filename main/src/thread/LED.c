#include <avr/io.h>
#include "LED.h"

#define SER_PORT    PORTD
#define SER_PIN     5
#define RCLK_PORT PORTD
#define RCLK_PIN  6
#define SRCLK_PORT PORTD
#define SRCLK_PIN  4

#define SER_low()  SER_PORT&=~_BV(SER_PIN)
#define SER_high() SER_PORT|=_BV(SER_PIN)
#define RCLK_low()  RCLK_PORT&=~_BV(RCLK_PIN)
#define RCLK_high() RCLK_PORT|=_BV(RCLK_PIN)
#define SRCLK_low()  SRCLK_PORT&=~_BV(SRCLK_PIN)
#define SRCLK_high() SRCLK_PORT|=_BV(SRCLK_PIN)

void LEDinit (void) {
    DDRD  |= 0b01110000; //1 = output, 0 = input
    PORTD &= 0b10001111;
}

void output_led_state(unsigned int __led_state) {
   SRCLK_low();
   RCLK_low();
   int i;
   for (i=0;i<8;i++)
   {
      if (bit_is_set(__led_state, i))
         SER_high();
      else   
         SER_low();
 

      SRCLK_high();
      SRCLK_low();
   }
   RCLK_high();
}

void LEDall(void) {
	output_led_state(0b11111111);
}

void LEDclear(void) {
	output_led_state(0b00000000);
}

void LEDnorth(void) {
	output_led_state(0b00100000);
}

void LEDnortheast(void) {
	output_led_state(0b00010000);
}

void LEDeast(void) {
	output_led_state(0b00001000);
}

void LEDsoutheast(void) {
	output_led_state(0b00000100);
}

void LEDsouth(void) {
	output_led_state(0b00000010);
}

void LEDsouthwest(void) {
	output_led_state(0b00000001);
}

void LEDwest(void) {
	output_led_state(0b10000000);
}

void LEDnorthwest(void) {
	output_led_state(0b01000000);
}




