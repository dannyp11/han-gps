/*
 * UIThread.c
 *
 *  Created on: Apr 10, 2017
 *      Author: dat
 */

#include "UIThread.h"
#include "LCD.h"
#include "LED.h"
#include "Button.h"
#include "Photocell.h"
#include "RotaryEncoder.h"
#include "Compass.h"
#include "buzzer.h"
#include "computationThread.h"

#include "chprintf.h"
	
#include <stdio.h>
#include <avr/interrupt.h>

static char UIMsg[21];
static char _compassDirection[10];
uint8_t mBrightnessLevel;
uint8_t isChanged;
uint8_t isOkPressed;
uint8_t isCancelPressed;

/*
 * Private variables
 */
static uint8_t g_friendID;
static CompassDirection g_friendCardinalDirection;
static uint8_t g_myMessageCode, g_friendMessageCode;
static float g_myLat, g_myLon;
static float g_friendLat, g_friendLon;

/*
 * all callbacks here
 */
static void photoCellCallback(uint8_t level)
{
	mBrightnessLevel = level;
}

static void buttonCancelCallback(void)
{

}

static void buttonOkCallback(void)
{

}

static void rotaryEncoderCallback(char value)
{

}

void UIShowLed(CompassDirection direction)
{
	switch (direction)
	{
	case NORTH:
		LEDnorth();
		break;
	case WEST:
		LEDwest();
		break;
	case EAST:
		LEDeast();
		break;
	case SOUTH:
		LEDsouth();
		break;
	case NORTHEAST:
		LEDnortheast();
		break;
	case NORTHWEST:
		LEDnorthwest();
		break;
	case SOUTHEAST:
		LEDsoutheast();
		break;
	case SOUTHWEST:
		LEDsouthwest();
		break;
	default:
		break;
	}
}

void UIInit(void)
{
	// init all vars
	mBrightnessLevel = 8;
	isChanged = 1;
	isCancelPressed = 0;
	isOkPressed = 0;

	// init all private vars
	g_myMessageCode = 0;
	g_friendMessageCode = 0;
	g_friendID = 1;
	g_friendCardinalDirection = EAST;
	g_myLat = 0;
	g_myLon = 0;
	g_friendLat = 0;
	g_friendLon = 0;

	// init all modules
	LCDInit();
	PhotocellInit();
	LEDinit();
	ButtonsInit();
	RotaryEncoderInit();
	buzzerInit();
	buzzOn();
	chThdSleepMilliseconds(200);
	buzzOff();

	// register all callbacks
	PhotocellRegisterCallback(photoCellCallback);
	ButtonCancelSetCallback(buttonCancelCallback);
	ButtonOKSetCallback(buttonOkCallback);
	RotaryEncoderSetCallback(rotaryEncoderCallback);

}

void UILoop(void)
{
	chsnprintf(UIMsg, 21, "My ID %d        ", g_myID);
	LCDSetCursor(1, 0);
	LCDPrint(UIMsg);


	CompassGetDirectionText(_compassDirection,
			CompassConvertToDirection(g_myCompassAngle));
	chsnprintf(UIMsg, 21, "Compass: %.0f %s    ",g_myCompassAngle,  _compassDirection);
	LCDSetCursor(3, 0);
	LCDPrint(UIMsg);

	UIShowLed(CompassConvertToDirection(g_myCompassAngle));
	LCDSetBrightness(mBrightnessLevel);
}

void UIAlertToFriends()
{

}

void UIAlertFromFriend(CompassDirection direction, uint8_t friendId,
		uint16_t distance)
{

}

void UIUpdateMyPosition(float lat, float lon)
{
	g_myLat = lat;
	g_myLon = lon;
}

void UIUpdateFriendInfo(uint8_t id, float lat, float lon,
		CompassDirection cardinalDirection)
{
	g_friendCardinalDirection = cardinalDirection;
	g_friendID = id;
	g_friendLat = lat;
	g_friendLon = lon;
}

float UIGetMyCompassDirection(void)
{
	return CompassGetAngle();
}

THD_WORKING_AREA(waTdUI, UI_WA_SIZE);

THD_FUNCTION(tdUI, arg)
{
	while (1)
	{
		UILoop();
		g_myCompassAngle = CompassGetAngle();
		chThdSleepMicroseconds(100);
	}
}

