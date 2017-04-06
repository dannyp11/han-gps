#pragma once

#include "debug.h"
#include "gpsParser.h"

#include "ch.h"

#define INVALID_XBEE_DATA -1

void xbeeStepParser(msg_t token);

/**
 * @brief Sets up mailbox and memory pool once a message is received. 
 */
void xbeeSetCallback(mailbox_t *mb, memory_pool_t *mp);

int8_t xbeeGetID(void);
deg_min_t xbeeGetLongitude(void);
deg_min_t xbeeGetLatitude(void);
int8_t xbeeGetMessage(void);