#include <avr/io.h>
#include <util/delay.h>

#include "LED.h"

void ioinit (void) {
    DDRD  |= 0b00011100; //1 = output, 0 = input
    PORTD &= 0b11100011;
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





