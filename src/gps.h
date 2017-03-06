#ifndef __GPS_H__
#define __GPS_H__

#include "ch.h"
#include "hal.h"
#include "softserialcfg.h"
#include "chprintf.h"

extern THD_WORKING_AREA(waTdGPS, 128);
extern THD_FUNCTION(tdGPS, arg);

/* TODO: DEBUG ONLY!*/
#define pGPS_SD &SD1
#define pGPSChs (BaseSequentialStream *)pGPS_SD
#define pGPSChn (BaseChannel *)pGPS_SD
#define pGPSEvt chnGetEventSource(pGPS_SD)

uint16_t getGPSLongitudeDeg(void);
uint16_t getGPSLongitudeMin(void);
uint16_t getGPSLatitudeDeg(void);
uint16_t getGPSLatitudeMin(void);
uint16_t getGPSSpeed(void);
uint16_t getGPSHeading(void);

#endif

