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

/*
 * Main global variables
 */
extern uint8_t g_myID, g_friendID;
extern float g_myLatitude, g_myLongitude;
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
void UIAlertFromFriend(alert_message_t *alerts, int8_t max_peers);

/*
 * This loops forever, should put in thread
 */
void UILoop(void);

#define UI_WA_SIZE 128
extern THD_WORKING_AREA(waTdUI, UI_WA_SIZE);
extern THD_FUNCTION(tdUI, arg);

#endif /* UITHREAD_H_ */
