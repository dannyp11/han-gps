#ifndef __GPS_H__
#define __GPS_H__

#include <inttypes.h>

#include "parser.h"
#include "debug.h"

#define GPS_WA_SIZE 128

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

void gpsStepParser(msg_t token);

#endif

