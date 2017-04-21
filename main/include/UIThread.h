/*
 * UIThread.h
 *
 *  Created on: Apr 10, 2017
 *      Author: dat
 */

#ifndef UITHREAD_H_
#define UITHREAD_H_

#include <stdint.h>
#include "ch.h"

/*
 * This will take care of photocell, LCD, LED
 * buttons, and menu navigation
 */
extern uint8_t g_myID;
extern float g_myCompassAngle;

/**
 * for easier life
 */
typedef struct _deviceInfo
{
	uint8_t id;
	float lat, lon;
	float compassAngle;
} DeviceInfo;

/*
 * Call this before using
 * This will also init lcd, buttons, compass, photocell
 */
void UIInit(void);

/*
 * Setters & getters, other threads can only call these
 */
void UIUpdateMyPosition(float lat, float lon); // gps should call this
void UIUpdateNearestFriendInfo(DeviceInfo friendInfo, float distance); // parser should call this
void UIAlertFromFriend(DeviceInfo friendInfo, float distance); // parser should call this
void UIAlertToFriends(void); // called when gps figures out that it's too far from closest friend

// TODO: Just for testing
void UISendMessage(float lat, float lon, int8_t msg);

/**
 * Thread stuff
 */
#define UI_WA_SIZE 160
extern THD_WORKING_AREA(waTdUI, UI_WA_SIZE);
extern THD_FUNCTION(tdUI, arg);

#endif /* UITHREAD_H_ */
