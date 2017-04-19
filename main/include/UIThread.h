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

/*
 * Call this before using
 * This will also init lcd, buttons, compass, photocell
 */
void UIInit(void);

/*
 * Setters & getters, other threads can only call these
 */
void UIUpdateMyPosition(float lat, float lon); // gps should call this
void UIUpdateNearestFriendInfo(uint8_t id, float lat, float lon, float friendCompassAngle); // parser should call this
void UIAlertFromFriend(uint8_t id, float lat, float lon, float friendCompassAngle); // parser should call this
void UIAlertToFriends(void); // called when gps figures out that it's too far from closest friend

/**
 * Thread stuff
 */
#define UI_WA_SIZE 128
extern THD_WORKING_AREA(waTdUI, UI_WA_SIZE);
extern THD_FUNCTION(tdUI, arg);

#endif /* UITHREAD_H_ */
