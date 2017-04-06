#pragma once

#include "ch.h"
#include "hal.h"
#include "xbeeParser.h"

#define XBEE_WA_SIZE 128

extern THD_WORKING_AREA(waTdXBee, XBEE_WA_SIZE);
extern THD_FUNCTION(tdXBEE, arg);

#define pXBEE_SD (SerialDriver *) &SD1
#define pXBEEChs (BaseSequentialStream *)pXBEE_SD
#define pXBEEChn (BaseChannel *)pXBEE_SD
#define pXBEEEvt (event_source_t *) chnGetEventSource(pXBEE_SD)