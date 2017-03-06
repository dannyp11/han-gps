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


#define INVALID_GPS_DATA (uint16_t) 0xFFFF

uint16_t getGPSLongitudeDeg(void);
uint16_t getGPSLongitudeMin(void);
uint16_t getGPSLatitudeDeg(void);
uint16_t getGPSLatitudeMin(void);

#endif

