
#ifndef BUZZER_H_
#define BUZZER_H_

#define BUZZ_PORT    PORTD
#define BUZZ_PIN     3

#define BUZZ_off()  BUZZ_PORT&=~_BV(BUZZ_PIN)
#define BUZZ_on() BUZZ_PORT|=_BV(BUZZ_PIN)


/*
Sets ATmega328P ports PD3 to output and
initializes buzzer to OFF
*/
void ioinit(void);

/*
Turns on buzzer
*/
void buzzOn(void);

/*
Turns off buzzer
*/
void buzzOff(void);


 #endif /* BUZZER_H_ */
