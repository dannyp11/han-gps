#ifndef _BOARD_CUSTOM_H_
#define  _BOARD_CUSTOM_H_

/* All inputs with pull-ups */
#define VAL_DDRB  0x00
#define VAL_PORTB 0xFF

/* PC0: TX.*/
#define VAL_DDRC  0x01
#define VAL_PORTC 0xFE

/*
 * PD2, PD3, PD4: LED
 */
#define VAL_DDRD  0b00001110
#define VAL_PORTD 0b11110001


#endif
