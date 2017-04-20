/*
 * UIThread.c
 *
 *  Created on: Apr 10, 2017
 *      Author: dat
 */

#include "UIThread.h"
#include "UIMessages.h"
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
#include <avr/eeprom.h>

/*
 * internal flags
 */
#define IS_CANCEL_PRESSED		0
#define IS_OK_PRESSED			1
#define IS_CHANGED				2
#define IS_CHANGING_CONTRAST	3
#define IS_PANICKING			4

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
static char UIMsgHolder[21];
static char _compassDirection[10];
static volatile uint8_t mBrightnessLevel;
static volatile uint8_t mContrastLevel;
static uint16_t uiFlags;
static int8_t mCurMenu;
static uint8_t mIsLooping;

/*
 * Private variables
 */
static DeviceInfo g_myDeviceInfo;
static DeviceInfo g_nearestFriendInfo, g_panicFriendInfo;
static float g_nearestFriendDistance, g_panicFriendDistance;

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

static void buttonUpCallback(void)
{

}

static void buttonDownCallback(void)
{

}

static void buttonCancelCallback(void)
{
	mCurMenu--;
	if (mCurMenu < 0)
		mCurMenu = 0;
	chThdSleepMilliseconds(500);
}

static void buttonOkCallback(void)
{
	mCurMenu++;
	if (mCurMenu >= MENU_COUNT)
		mCurMenu = MENU_COUNT - 1;
	chThdSleepMilliseconds(500);
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
	UISetFlag(IS_PANICKING, 0);
	mCurMenu = MY_INFO;

	// init all private vars
	g_nearestFriendDistance = 10.1f;
	g_panicFriendDistance = 101.1f;
	g_nearestFriendInfo.id = 1;
	g_nearestFriendInfo.lat = 2;
	g_nearestFriendInfo.lon = 3;
	g_nearestFriendInfo.compassAngle = 270.0;
	g_panicFriendInfo.id = 2;
	g_panicFriendInfo.lat = 4;
	g_panicFriendInfo.lon = 6;
	g_panicFriendInfo.compassAngle = 60.0;
	g_myDeviceInfo.id = g_myID;
	g_myDeviceInfo.lat = 3;
	g_myDeviceInfo.lon = 5;
	g_myCompassAngle = 0.0f;

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
	PgmStorageGet(UIMsg, UIMyInfo1);
	chsnprintf(UIMsg, 21, UIMsg, g_myID);
	LCDSetCursor(1, 0);
	LCDPrint(UIMsg);

	PgmStorageGet(UIMsg, UIMyInfo2);
	chsnprintf(UIMsg, 21, UIMsg, g_myDeviceInfo.lat, g_myDeviceInfo.lon);
	LCDSetCursor(2, 0);
	LCDPrint(UIMsg);

	CompassGetDirectionText(_compassDirection,
			CompassConvertToDirection(360 - g_myCompassAngle));
	PgmStorageGet(UIMsgHolder, UIMyInfo3);
	chsnprintf(UIMsg, 21, UIMsgHolder, _compassDirection);
	LCDSetCursor(3, 0);
	LCDPrint(UIMsg);

	PgmStorageGet(UIMsgHolder, UIMyInfo4);
	chsnprintf(UIMsg, 21, UIMsgHolder, g_myCompassAngle);
	LCDSetCursor(4, 0);
	LCDPrint(UIMsg);

	UIShowLed(CompassConvertToDirection(g_myCompassAngle));
}

void UI_ShowFriendInfo(void)
{
	float cardinalAngle = g_myCompassAngle - g_nearestFriendInfo.compassAngle;
	if (cardinalAngle < 0)
		cardinalAngle = 360 - cardinalAngle;

	PgmStorageGet(UIMsg, UIFriendInfo1);
	chsnprintf(UIMsg, 21, UIMsg, g_nearestFriendInfo.id);
	LCDSetCursor(1, 0);
	LCDPrint(UIMsg);

	PgmStorageGet(UIMsg, UIFriendInfo2);
	chsnprintf(UIMsg, 21, UIMsg, g_nearestFriendInfo.lat,
			g_nearestFriendInfo.lon);
	LCDSetCursor(2, 0);
	LCDPrint(UIMsg);

	CompassGetDirectionText(_compassDirection,
			CompassConvertToDirection(360 - cardinalAngle));
	PgmStorageGet(UIMsgHolder, UIFriendInfo3);
	chsnprintf(UIMsg, 21, UIMsgHolder, _compassDirection);
	LCDSetCursor(3, 0);
	LCDPrint(UIMsg);

	PgmStorageGet(UIMsgHolder, UIFriendInfo4);
	chsnprintf(UIMsg, 21, UIMsgHolder, g_nearestFriendDistance);
	LCDSetCursor(4, 0);
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
	chsnprintf(UIMsg, 21, "Panic mode           ", g_myID);
	LCDSetCursor(1, 0);
	LCDPrint(UIMsg);

	if (UIGetFlag(IS_PANICKING))
	{
		chsnprintf(UIMsg, 21, "OMG I'm lost!!!       ", g_myID);
		LCDSetCursor(2, 0);
		LCDPrint(UIMsg);

		buzzOn();
		chThdSleepMilliseconds(500);
		buzzOff();
		chThdSleepMilliseconds(500);

		chsnprintf(UIMsg, 21, "Press cancel to unpanic       ", g_myID);
		LCDSetCursor(3, 0);
		LCDPrint(UIMsg);

		// call xbee emergency api
	}
	else
	{
		chsnprintf(UIMsg, 21, "Press Ok to activate       ", g_myID);
		LCDSetCursor(2, 0);
		LCDPrint(UIMsg);

		// call xbee unemergency api
	}
}

void UI_ShowContrastSettings(void)
{
	chsnprintf(UIMsg, 21, "Contrast setting value %d      ", mContrastLevel);
	LCDSetCursor(1, 0);
	LCDPrint(UIMsg);

	chsnprintf(UIMsg, 21, "Ok/Cancel to change          ");
	LCDSetCursor(2, 0);
	LCDPrint(UIMsg);

	LCDSetContrast(mContrastLevel);
}

void UIAlertToFriends()
{

}

void UIAlertFromFriend(DeviceInfo friendInfo, float distance)
{
	g_panicFriendInfo = friendInfo;
	g_panicFriendDistance = distance;
}

void UIUpdateMyPosition(float lat, float lon)
{
	g_myDeviceInfo.lat = lat;
	g_myDeviceInfo.lon = lon;
}

void UIUpdateNearestFriendInfo(DeviceInfo friendInfo, float distance)
{
	g_nearestFriendInfo = friendInfo;
	g_nearestFriendDistance = distance;
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

