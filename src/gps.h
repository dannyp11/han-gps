#ifndef __GPS_H__
#define __GPS_H__

#include "ch.h"
#include "hal.h"
#include "softserialcfg.h"
#include "chprintf.h"

#define GPS_WA_SIZE 128

extern THD_WORKING_AREA(waTdGPS, GPS_WA_SIZE);
extern THD_FUNCTION(tdGPS, arg);

/* TODO: DEBUG ONLY!*/
#define pGPS_SD (SerialDriver *) &SD1
#define pGPSChs (BaseSequentialStream *)pGPS_SD
#define pGPSChn (BaseChannel *)pGPS_SD
#define pGPSEvt (event_source_t *) chnGetEventSource(pGPS_SD)

#endif

