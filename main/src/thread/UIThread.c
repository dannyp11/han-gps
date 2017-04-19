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

/*
 * internal flags
 */
#define IS_CANCEL_PRESSED		0
#define IS_OK_PRESSED			1
#define IS_CHANGED				2
#define IS_CHANGING_CONTRAST	3

/**
 * Menu ID
 */
typedef enum _UI_Menu
{
	MY_INFO = 0,
	FRIEND_INFO,
	PANIC_MODE,
	FRIEND_ALERT,
	CONTRAST_SETTING,
	MENU_COUNT
} UI_Menu;

static char UIMsg[21];
static char _compassDirection[10];
static volatile uint8_t mBrightnessLevel;
static volatile uint8_t mContrastLevel;
static uint16_t uiFlags;
static UI_Menu mCurMenu;
static uint8_t mIsLooping;

/*
 * Private variables
 */
static uint8_t g_friendID;
static CompassDirection g_friendCardinalDirection;
static uint8_t g_myMessageCode, g_friendMessageCode;
static float g_myLat, g_myLon;
static float g_friendLat, g_friendLon;
static float g_myLat, g_myLon;
static float g_friendAngle;

/**
 * Flags manipulation
 */
void UISetFlag(uint8_t flag, uint8_t val)
{
	if (val)
		uiFlags |= (1 << flag);
	else
		uiFlags &= ~(1 << flag);
}
uint8_t UIGetFlag(uint8_t flag)
{
	return (uiFlags & (1 << flag)) >> flag;
}

/*
 * all callbacks here
 */
static void photoCellCallback(uint8_t level)
{
	mBrightnessLevel = level;
}

static void buttonCancelCallback(void)
{
	mCurMenu--;
}

static void buttonOkCallback(void)
{
	mCurMenu++;
}

static void rotaryEncoderCallback(char value)
{
	if (value > 0)
		++mContrastLevel;
	else
		--mContrastLevel;
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
	mIsLooping = 1;
	uiFlags = 0;
	mBrightnessLevel = 8;
	mContrastLevel = 40;
	UISetFlag(IS_CANCEL_PRESSED, 0);
	UISetFlag(IS_OK_PRESSED, 0);
	UISetFlag(IS_CHANGED, 1);
	UISetFlag(IS_CHANGING_CONTRAST, 0);
	mCurMenu = MY_INFO;

	// init all private vars
	g_myMessageCode = 0;
	g_friendMessageCode = 0;
	g_friendID = 1;
	g_friendCardinalDirection = EAST;
	g_myLat = 0;
	g_myLon = 0;
	g_friendLat = 0;
	g_friendLon = 0;
	g_friendAngle = 90;

	// init all modules
	LCDInit();
	CompassInit();
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

void UI_SHowMyInfo(void)
{
	chsnprintf(UIMsg, 21, "My ID %d        ", g_myID);
	LCDSetCursor(1, 0);
	LCDPrint(UIMsg);

	chsnprintf(UIMsg, 21, "Lat %.2f Lon %.2f     ", g_myLat, g_myLon);
	LCDSetCursor(2, 0);
	LCDPrint(UIMsg);

	CompassGetDirectionText(_compassDirection,
			CompassConvertToDirection(360 - g_myCompassAngle));
	chsnprintf(UIMsg, 21, "Direction %s               ", _compassDirection);
	LCDSetCursor(3, 0);
	LCDPrint(UIMsg);

	chsnprintf(UIMsg, 21, "Angle %.2f                 ", g_myCompassAngle);
	LCDSetCursor(4, 0);
	LCDPrint(UIMsg);

	UIShowLed(CompassConvertToDirection(g_myCompassAngle));
}

void UI_ShowFriendInfo(void)
{
	float cardinalAngle = g_myCompassAngle - g_friendAngle;
	if (cardinalAngle < 0)
		cardinalAngle = 360 - cardinalAngle;

	chsnprintf(UIMsg, 21, "Friend ID %d        ", g_friendID);
	LCDSetCursor(1, 0);
	LCDPrint(UIMsg);

	chsnprintf(UIMsg, 21, "Lat %.2f Lon %.2f     ", g_friendLat, g_friendLon);
	LCDSetCursor(2, 0);
	LCDPrint(UIMsg);

	CompassGetDirectionText(_compassDirection,
			CompassConvertToDirection(360 - cardinalAngle));
	chsnprintf(UIMsg, 21, "Direction %s         ", _compassDirection);
	LCDSetCursor(3, 0);
	LCDPrint(UIMsg);

	UIShowLed(CompassConvertToDirection(cardinalAngle));
}

void UI_ShowFriendAlert(void)
{
	chsnprintf(UIMsg, 21, "Friend Alert           ", g_myID);
	LCDSetCursor(1, 0);
	LCDPrint(UIMsg);
}

void UI_ShowPanicMode(void)
{
	chsnprintf(UIMsg, 21, "Panic!           ", g_myID);
	LCDSetCursor(1, 0);
	LCDPrint(UIMsg);
}

void UI_ShowContrastSettings(void)
{
	chsnprintf(UIMsg, 21, "Contrast setting          ");
	LCDSetCursor(1, 0);
	LCDPrint(UIMsg);

	chsnprintf(UIMsg, 21, "Rotate to change          ");
	LCDSetCursor(2, 0);
	LCDPrint(UIMsg);

	LCDSetContrast(mContrastLevel);
}

void UIAlertToFriends()
{

}

void UIAlertFromFriend(uint8_t id, float lat, float lon,
		float friendCompassAngle)
{

}

void UIUpdateMyPosition(float lat, float lon)
{
	g_myLat = lat;
	g_myLon = lon;
}

void UIUpdateFriendInfo(uint8_t id, float lat, float lon,
		float friendCompassAngle)
{
	g_friendID = id;
	g_friendLat = lat;
	g_friendLon = lon;
}

THD_WORKING_AREA(waTdUI, UI_WA_SIZE);
THD_FUNCTION(tdUI, arg)
{
	static UI_Menu prev_mCurMenu = MENU_COUNT;
	static int i = 0;

	while (1)
	{
		// update values
		g_myCompassAngle = 360 - CompassGetAngle();
		if (mCurMenu < 0)
			mCurMenu = 0;
		else if (mCurMenu > MENU_COUNT)
			mCurMenu = MENU_COUNT - 1;

		// clearscr if changed menu
		if (prev_mCurMenu != mCurMenu)
		{
			LCDReset();
		}

		// state machine of menu
		switch (mCurMenu)
		{
		case MY_INFO:
			UI_SHowMyInfo();
			break;
		case FRIEND_INFO:
			UI_ShowFriendInfo();
			break;
		case FRIEND_ALERT:
			UI_ShowFriendAlert();
			break;
		case PANIC_MODE:
			UI_ShowPanicMode();
			break;
		case CONTRAST_SETTING:
			UI_ShowContrastSettings();
			break;
		default:
			break;
		}

		if (mBrightnessLevel < 2)
			mBrightnessLevel = 2;
		LCDSetBrightness(mBrightnessLevel);
		prev_mCurMenu = mCurMenu;
		chThdSleepMilliseconds(300);
	}
}

