#ifndef __GPS_H__
#define __GPS_H__

#include "ch.h"
#include "hal.h"
#include "softserialcfg.h"
#include "chprintf.h"

#if DEBUG_GPS || DEBUG_PARSER
#define GPS_WA_SIZE 128
#else 
#define GPS_WA_SIZE 128
#endif

extern THD_WORKING_AREA(waTdGPS, GPS_WA_SIZE);
extern THD_FUNCTION(tdGPS, arg);

#define pGPS_SD (SerialDriver *) &SDS
#define pGPSChs (BaseSequentialStream *)pGPS_SD
#define pGPSChn (BaseChannel *)pGPS_SD
#define pGPSEvt (event_source_t *) chnGetEventSource(pGPS_SD)

#endif

