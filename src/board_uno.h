#ifndef _BOARD_UNO_H_
#define _BOARD_UNO_H_

/* All inputs with pull-ups, except PB5 which has an LED.*/
#define VAL_DDRB  0x10
#define VAL_PORTB 0xEF

/* All inputs with pull-ups.*/
#define VAL_DDRC  0x00
#define VAL_PORTC 0xFF

/* All inputs except PD1 (Serial TX0) */
#define VAL_DDRD  0x02
#define VAL_PORTD 0xFF

#define TEST_LED_PORT IOPORT2
#define TEST_LED_PIN  5

#endif
