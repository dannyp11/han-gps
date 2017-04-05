#pragma once

#include "debug.h"
#include "gpsParser.h"

#include "ch.h"

#define INVALID_XBEE_DATA -1

void xbeeStepParser(msg_t token);

/**
 * @brief Sets up mailbox once a message is received. 
 */
void xbeeSetCallbackThread(mailbox_t *mb);

int8_t xbeeGetID(void);
deg_min_t xbeeGetLongitude(void);
deg_min_t xbeeGetLatitude(void);
int8_t xbeeGetMessage(void);