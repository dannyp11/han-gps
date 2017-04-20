/*
 * UIMessages.h
 *
 *  Created on: Apr 20, 2017
 *      Author: danny
 */

#ifndef INCLUDE_UIMESSAGES_H_
#define INCLUDE_UIMESSAGES_H_

#include "PgmStorage.h"

/**
 * show my info menu
 */
const char PROGMEM UIMyInfo1[] = "My ID %d        ";
const char PROGMEM UIMyInfo2[] = "Lat %.2f Lon %.2f     ";
const char PROGMEM UIMyInfo3[] = "Direction %s        ";
const char PROGMEM UIMyInfo4[] = "Angle %.2f          ";

/**
 * show friend info menu
 */
const char PROGMEM UIFriendInfo1[] = "Friend ID %d        ";
const char PROGMEM UIFriendInfo2[] = "Lat %.2f Lon %.2f     ";
const char PROGMEM UIFriendInfo3[] = "Direction %s         ";
const char PROGMEM UIFriendInfo4[] = "Distance %.1f       ";

/**
 * panic mode
 */
const char PROGMEM UIPanicMode1[] = "Panic mode         ";
const char PROGMEM UIPanicMode2[] = "OMG I'm Lost         ";
const char PROGMEM UIPanicMode3[] = "Press cancel to unpanic       ";
const char PROGMEM UIPanicMode4[] = "Press ok to panic         ";

/**
 * show friend alert
 */
const char PROGMEM UIFriendAlert1[] = "Friend Alert         ";

/**
 * contrast settings
 */
const char PROGMEM UIContrastSetting1[] = "Contrast setting value %d      ";
const char PROGMEM UIContrastSetting2[] = "Ok/Cancel to change          ";

#endif /* INCLUDE_UIMESSAGES_H_ */

