/*
 * RotaryEncoder.h
 *
 *  Created on: Mar 2, 2017
 *      Author: dat
 */

#ifndef ROTARYENCODER_H_
#define ROTARYENCODER_H_

// Callback function prototype
typedef void (*RotaryEncoderChangeCallback)(char);

/*
 * Call this before using APIs
 */
void RotaryEncoderInit();

/*
 * Register callback function to be called when rotary encoder changes
 * The char value is -1 if CCW, 1 if CW, 0 if error
 * Currently only supports 1 callback
 * eg. void printVal(char val)
 *
 * return 0 on success, -1 if there's existing registered function
 */
uint8_t RotaryEncoderSetCallback(RotaryEncoderChangeCallback function);

#endif /* ROTARYENCODER_H_ */
