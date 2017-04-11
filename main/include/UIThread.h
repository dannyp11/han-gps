/*
 * UIThread.h
 *
 *  Created on: Apr 10, 2017
 *      Author: dat
 */

#ifndef UITHREAD_H_
#define UITHREAD_H_

#include <stdint.h>
#include "Compass.h"
/*
 * This will take care of photocell, LCD, LED
 * buttons, and menu navigation
 */

/*
 * Main global variables
 */
extern uint8_t g_myID, g_friendID;
extern uint8_t g_myLatitude, g_myLongtitude;
extern CompassDirection g_myCompassDirection, g_friendCompassDirection;
extern CompassDirection g_friendCardinalDirection;
extern uint8_t g_myMessageCode, g_friendMessageCode;

/*
 * Call this before using
 * This will also init lcd, buttons, compass, photocell
 */
void UIInit(void);

/*
 * Show alert info to neighbor
 */
void UIAlertToFriend(uint8_t friendID);

/*
 * Show alert info from neighbor
 */
void UIAlertFromFriend(uint8_t friendID, uint8_t friendLat, uint8_t friendLon);

/*
 * This loops forever, should put in thread
 */
void UILoop(void);

#endif /* UITHREAD_H_ */
