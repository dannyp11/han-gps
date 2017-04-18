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
#include "Compass.h"
#include "computationThread.h"

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
 * Show alert info to neighbor
 * called when gps figures out that it's too far from closest friend
 */
void UIAlertToFriends(void);

/*
 * Show alert info from neighbor
 */
void UIAlertFromFriend(CompassDirection direction, uint8_t friendId, uint16_t distance);

/*
 * This loops forever, should put in thread
 */
void UILoop(void);

/*
 * Setters & getters
 */
void UIUpdateMyPosition(float lat, float lon);
void UIUpdateFriendInfo(uint8_t id, float lat, float lon, CompassDirection cardinalDirection);
float UIGetMyCompassAngle(void);

#define UI_WA_SIZE 128
extern THD_WORKING_AREA(waTdUI, UI_WA_SIZE);
extern THD_FUNCTION(tdUI, arg);

#endif /* UITHREAD_H_ */
