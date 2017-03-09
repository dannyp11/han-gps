#include <avr/io.h>
#include <util/delay.h>

#define SER_PORT    PORTD
#define SER_PIN     2
#define RCLK_PORT PORTD
#define RCLK_PIN  3
#define SRCLK_PORT PORTD
#define SRCLK_PIN  4
 
#define SER_low()  SER_PORT&=~_BV(SER_PIN)
#define SER_high() SER_PORT|=_BV(SER_PIN)
#define RCLK_low()  RCLK_PORT&=~_BV(RCLK_PIN)
#define RCLK_high() RCLK_PORT|=_BV(RCLK_PIN)
#define SRCLK_low()  SRCLK_PORT&=~_BV(SRCLK_PIN)
#define SRCLK_high() SRCLK_PORT|=_BV(SRCLK_PIN)

//Define functions
//===============================================
void ioinit(void);
void output_led_state(unsigned int __led_state);
//===============================================



int main(void) {
	
	ioinit(); //Setup IO pins and defaults
	int i;

	while (1) {
			for (i=7;i>=0;i--)
      {
         output_led_state(_BV(i));
         _delay_ms(200);
      }
	}

	return 0; /* never reached */
}

void ioinit (void)
{
    DDRD  = 0b00011100; //1 = output, 0 = input
    PORTD = 0b00000000;
}

void output_led_state(unsigned int __led_state)
{
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