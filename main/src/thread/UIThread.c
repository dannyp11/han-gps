/*
 * UIThread.c
 *
 *  Created on: Apr 10, 2017
 *      Author: dat
 */

#include "UIThread.h"

#include "Button.h"
#include "Compass.h"
#include "LCD.h"
#include "LED.h"
#include "Photocell.h"
#include "RotaryEncoder.h"
#include "UIMessages.h"
#include "buzzer.h"
#include "computationThread.h"

#include "chprintf.h"

#include <avr/eeprom.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <math.h>

/*
 * internal flags
 */
#define IS_CANCEL_PRESSED 0
#define IS_OK_PRESSED 1
#define IS_CHANGED 2
#define IS_CHANGING_CONTRAST 3
#define IS_PANICKING 4
#define IS_FRIEND_PANICKING	5
#define IS_SELFDETECT_PANICKING	6

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
	mCurMenu++;
	if (mCurMenu >= MENU_COUNT)
		mCurMenu = MENU_COUNT - 1;
}

static void buttonDownCallback(void)
{
	mCurMenu--;
	if (mCurMenu < 0)
		mCurMenu = 0;
}

static void buttonCancelCallback(void)
{
	if (mCurMenu == PANIC_MODE)
	{
		UISetFlag(IS_PANICKING, 0);
	}
	else if (mCurMenu == FRIEND_ALERT)
	{
		UISetFlag(IS_FRIEND_PANICKING, 0);
	}
	else if (mCurMenu == CONTRAST_SETTING)
	{
		mContrastLevel--;
		if (mContrastLevel == 0)
			mContrastLevel = 0;
	}
}

static void buttonOkCallback(void)
{
	if (mCurMenu == PANIC_MODE)
	{
		UISetFlag(IS_PANICKING, 1);
	}
	else if (mCurMenu == CONTRAST_SETTING)
	{
		mContrastLevel++;
		if (mContrastLevel > 50)
			mContrastLevel = 50;
	}
}

static void rotaryEncoderCallback(char value)
{
	if (value > 0)
		++mContrastLevel;
	else
		--mContrastLevel;
}

void UISetEmergencyBuzzer(uint8_t mode)
{
	if (mode)
	{
		buzzOn();
		chThdSleepMilliseconds(700);
		buzzOff();
		chThdSleepMilliseconds(300);
	}
	else
	{
		buzzOff();
	}
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
	UISetFlag(IS_FRIEND_PANICKING, 0);
	UISetFlag(IS_SELFDETECT_PANICKING, 0);
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
	chThdSleepMilliseconds(50);
	buzzOff();

	// register all callbacks
	PhotocellRegisterCallback(&photoCellCallback);
	ButtonCancelSetCallback(&buttonCancelCallback);
	ButtonOKSetCallback(buttonOkCallback);
	ButtonDownSetCallback(buttonDownCallback);
	ButtonUpSetCallback(buttonUpCallback);
	RotaryEncoderSetCallback(rotaryEncoderCallback);

	// testing area
//	mCurMenu = FRIEND_ALERT;
//	UISetFlag(IS_FRIEND_PANICKING, 1);
}

void UI_SHowMyInfo(void)
{
	PgmStorageGet(UIMsg, UIMyInfo1);
	chsnprintf(UIMsg, 21, UIMsg, g_myID);
	LCDSetCursor(1, 0);
	LCDPrint(UIMsg);

	PgmStorageGet(UIMsgHolder, UIMyInfo2);
	chsnprintf(UIMsg, 21, UIMsgHolder, g_myDeviceInfo.lat, g_myDeviceInfo.lon);
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
		cardinalAngle = 360 + cardinalAngle;

	PgmStorageGet(UIMsgHolder, UIFriendInfo1);
	chsnprintf(UIMsg, 21, UIMsgHolder, g_nearestFriendInfo.id);
	LCDSetCursor(1, 0);
	LCDPrint(UIMsg);

	PgmStorageGet(UIMsgHolder, UIFriendInfo2);
	chsnprintf(UIMsg, 21, UIMsgHolder, g_nearestFriendInfo.lat,
			g_nearestFriendInfo.lon);
	LCDSetCursor(2, 0);
	LCDPrint(UIMsg);

	CompassGetDirectionText(_compassDirection,
			CompassConvertToDirection(cardinalAngle));
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
	if (UIGetFlag(IS_FRIEND_PANICKING) == 0)
	{
		LCDSetCursor(1, 0);
		LCDPrint("No friend alerting             ");

		PgmStorageGet(UIMsg, UIBlank);
		LCDSetCursor(2, 0);
		LCDPrint(UIMsg);
		LCDSetCursor(3, 0);
		LCDPrint(UIMsg);
		LCDSetCursor(4, 0);
		LCDPrint(UIMsg);
		return;
	}

	PgmStorageGet(UIMsgHolder, UIFriendAlert1);
	chsnprintf(UIMsg, 21, UIMsgHolder, g_panicFriendInfo.id);
	LCDSetCursor(1, 0);
	LCDPrint(UIMsg);

	chsnprintf(UIMsg, 21, "Lat %.2f Lon %.2f       ", g_panicFriendInfo.lat,
			g_panicFriendInfo.lon);
	LCDSetCursor(2, 0);
	LCDPrint(UIMsg);

	chsnprintf(UIMsg, 21, "Distance %.1f meters     ", g_panicFriendDistance);
	LCDSetCursor(3, 0);
	LCDPrint(UIMsg);

	LCDSetCursor(4, 0);
	LCDPrint("Press cancel to ignore         ");

	buzzOn();
	chThdSleepMilliseconds(200);
	buzzOff();
	chThdSleepMilliseconds(800);
}

void UI_ShowPanicMode(void)
{
	PgmStorageGet(UIMsgHolder, UIPanicMode1);
	chsnprintf(UIMsg, 21, UIMsgHolder);
	LCDSetCursor(1, 0);
	LCDPrint(UIMsg);

	if (UIGetFlag(IS_PANICKING))
	{
		PgmStorageGet(UIMsgHolder, UIPanicMode2);
		chsnprintf(UIMsg, 21, UIMsgHolder);
		LCDSetCursor(2, 0);
		LCDPrint(UIMsg);

		UISetEmergencyBuzzer(1);

		PgmStorageGet(UIMsgHolder, UIPanicMode3);
		chsnprintf(UIMsg, 21, UIMsgHolder);
		LCDSetCursor(3, 0);
		LCDPrint(UIMsg);

		// call xbee emergency api
	}
	else
	{
		PgmStorageGet(UIMsgHolder, UIPanicMode4);
		chsnprintf(UIMsg, 21, UIMsgHolder);
		LCDSetCursor(2, 0);
		LCDPrint(UIMsg);

		PgmStorageGet(UIMsg, UIBlank);
		LCDSetCursor(3, 0);
		LCDPrint(UIMsg);
		LCDSetCursor(4, 0);
		LCDPrint(UIMsg);

		UISetEmergencyBuzzer(0);

		// call xbee unemergency api
	}
}

void UI_ShowContrastSettings(void)
{
	PgmStorageGet(UIMsgHolder, UIContrastSetting1);
	chsnprintf(UIMsg, 21, UIMsgHolder, mContrastLevel);
	LCDSetCursor(1, 0);
	LCDPrint(UIMsg);

	PgmStorageGet(UIMsg, UIContrastSetting2);
	chsnprintf(UIMsg, 21, UIMsg);
	LCDSetCursor(2, 0);
	LCDPrint(UIMsg);

	PgmStorageGet(UIMsg, UIBlank);
	LCDSetCursor(3, 0);
	LCDPrint(UIMsg);
	LCDSetCursor(4, 0);
	LCDPrint(UIMsg);

	LCDSetContrast(mContrastLevel);
}

void UIAlertToFriends()
{
	if (!UIGetFlag(IS_SELFDETECT_PANICKING))
	{
		UISetFlag(IS_SELFDETECT_PANICKING, 1);
		mCurMenu = PANIC_MODE;
		UISetFlag(IS_PANICKING, 1);
	}
}

void UIAlertFromFriend(DeviceInfo friendInfo, float distance)
{
	mCurMenu = FRIEND_ALERT;
	UISetFlag(IS_FRIEND_PANICKING, 1);
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

void UISendMessage(float lat, float lon, int8_t msg)
{
	char sendbuf[29];
	int16_t longDeg, longMin, longSec, latDeg, latMin, latSec;

	longDeg = (int16_t) truncf(lon);
	latDeg = (int16_t) truncf(lat);

	{
		float tmp = (lon * 180 / M_PI - longDeg) * 60;
		longMin = (int16_t) truncf(tmp);
		longSec = (int16_t) ((tmp - longMin) * 10000.f);
		tmp = (lat * 180 / M_PI - latDeg) * 60;
		latMin = (int16_t) truncf(tmp);
		latSec = (int16_t) ((tmp - latMin) * 10000.f);
	}

	chsnprintf(sendbuf, 29, "#%02d,%03d%02d%04d,%03d%02d%04d,%d$", g_myID,
			longDeg, longMin, longSec, latDeg, latMin, latSec, msg);

	chprintf((BaseSequentialStream *) &SD1, sendbuf);
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
