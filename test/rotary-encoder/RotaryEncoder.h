/*
 * RotaryEncoder.h
 *
 *  Created on: Mar 2, 2017
 *      Author: dat
 */

#ifndef ROTARYENCODER_H_
#define ROTARYENCODER_H_

// Callback function prototype
typedef void (*CallbackFunction)(uint16_t);

/*
 * Feel free to change these values
 */
#define ROTARYENCODER_MIN_VALUE 100
#define ROTARYENCODER_MAX_VALUE 300

/*
 * Call this before using APIs
 */
void RotaryEncoderInit();

/*
 * Get value from encoder, encouraged to minimize calling this as oppose to callback
 * return -1 if error
 */
uint16_t RotaryEncoderGetVal();

/*
 * Register callback function to be called when rotary encoder gets net value
 * Currently only supports 1 callback
 * eg. void printVal(int val)
 *
 * return 0 on success, -1 if there's existing registered function
 */
uint8_t RotaryEncoderSetCallback(CallbackFunction function);

#endif /* ROTARYENCODER_H_ */
