/*
 * UIThread.c
 *
 *  Created on: Apr 10, 2017
 *      Author: dat
 */

#include "UIThread.h"
#include "LCD.h"
#include "Button.h"
#include "Photocell.h"
#include "RotaryEncoder.h"
#include "Compass.h"
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

void UIInit(void)
{
	// init all vars
	mBrightnessLevel = 8;
	isChanged = 1;
	isCancelPressed = 0;
	isOkPressed = 0;

	// init all modules
	LCDInit();
	PhotocellInit();
	ButtonsInit();
	RotaryEncoderInit();

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

	chsnprintf(UIMsg, 21, "La %d Lo %d      ", g_myLatitude, g_myLongtitude);
	LCDSetCursor(2, 0);
	LCDPrint(UIMsg);

	CompassGetDirectionText(_compassDirection, g_myCompassDirection);
	chsnprintf(UIMsg, 21, "Compass: %s       ", _compassDirection);
	LCDSetCursor(3, 0);
	LCDPrint(UIMsg);

	LCDSetBrightness(mBrightnessLevel);
}

void UIAlertToFriend(uint8_t friendID)
{

}

void UIAlertFromFriend(alert_message_t *alerts, int8_t max_peers)
{

}

THD_WORKING_AREA(waTdUI, UI_WA_SIZE);
THD_FUNCTION(tdUI, arg) {

  while (1) {
    UILoop();
    g_myCompassDirection = CompassGetDirection();
    chThdSleepMicroseconds(100);
  }
}