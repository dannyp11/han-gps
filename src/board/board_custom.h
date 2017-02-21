#ifndef _BOARD_CUSTOM_H_
#define  _BOARD_CUSTOM_H_

/* All inputs with pull-ups */
#define VAL_DDRB  0x00
#define VAL_PORTB 0xFF

/* All inputs with pull-ups except PC0 which has an LED */
#define VAL_DDRC  0x01
#define VAL_PORTC 0xFE

/* All inputs except PD1 (Serial TX0) */
#define VAL_DDRD  0x02
#define VAL_PORTD 0xFF

#define TEST_LED_PORT IOPORT3
#define TEST_LED_PIN  0

#endif
