/*
 * Compass.h
 *
 *  Created on: Feb 28, 2017
 *      Author: dat
 */

#ifndef COMPASS_H_
#define COMPASS_H_

typedef enum compassDirection
{
	NORTH,
	EAST,
	SOUTH,
	WEST,
	NORTHEAST,
	NORTHWEST,
	SOUTHEAST,
	SOUTHWEST
} CompassDirection;

/*
 * Call this function to initialize the Compass
 */
void CompassInit(void);

/*
 * main getter here
 */
CompassDirection CompassGetDirection(void);

/*
 * convert from angle to direction
 */
CompassDirection CompassConvertToDirection(float angle);

/*
 * Helper for converting CompassDirection to text
 * print the text to buffer, make sure buffer is at least 10 length
 * return strlen of the printed buffer
 */
uint8_t CompassGetDirectionText(char * buffer, CompassDirection direction);

/*
 * secondary getter
 * Note: this is raw angle, not comparing to true North
 * for calibrated direction, use CompassGetDirection
 */
float CompassGetAngle(void);

#endif /* COMPASS_H_ */
