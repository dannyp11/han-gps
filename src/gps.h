#ifndef __GPS_H__
#define __GPS_H__

#include "nil.h"
#include "hal.h"
#include "softserialcfg.h"
#include "chprintf.h"

extern THD_WORKING_AREA(waTdGPS, 128);
THD_FUNCTION(tdGPS, arg);

uint16_t getGPSLongitudeDeg(void);
uint16_t getGPSLongitudeMin(void);
uint16_t getGPSLatitudeDeg(void);
uint16_t getGPSLatitudeMin(void);
uint16_t getGPSSpeed(void);
uint16_t getGPSHeading(void);

#endif

