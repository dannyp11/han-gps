#pragma once 

#include "ch.h"
//#include "hal.h"
#include "LCD_ll.h"

extern void LCDClear(void);

/*
 * LCDPrint - write char array to the LCD over the I2C bus
 * input: msg
 * return value: 0 on success
 */
extern uint8_t LCDPrint(const char* msg);

/*
 * Pretty straightforward: send command in the LCDCommand enum above
 */
extern uint8_t LCDSendCommand(enum LCDCommand command);

/*
 * Set brightness of the LCD, level can be 1 to 8, default is 8
 */
extern uint8_t LCDSetBrightness(char level);

/*
 * Set contrast of the LCD, level can be 1 to 50, default is 40
 */
extern uint8_t LCDSetContrast(char level);

/*
 * Set position and line number of the cursor
 * line: 1-4
 * position: 0-19
 */
extern uint8_t LCDSetCursor(char line, char position);

#define LCD_WA_SIZE 128
extern THD_WORKING_AREA(waTdLCD, LCD_WA_SIZE);
extern THD_FUNCTION(tdLCD, arg);