#pragma once

#include "debug.h"
#include "gpsParser.h"

#include "ch.h"

#define INVALID_XBEE_DATA -1

void xbeeStepParser(msg_t token);

/**
 * @brief Sets up callback once a message is received.
 */
void xbeeSetCallbackThread(thread_t *td);

int8_t xbeeGetID(void);
deg_min_t xbeeGetLongitude(void);
deg_min_t xbeeGetLatitude(void);
int8_t xbeeGetMessage(void);