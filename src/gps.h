#ifndef __GPS_H__
#define __GPS_H__

#include "nil.h"
#include "hal.h"
#include "softserialcfg.h"

extern THD_WORKING_AREA(waTdGPS, 128);
THD_FUNCTION(td_gps, arg);

uint16_t getGPSLongitudeDeg();
uint16_t getGPSLongitudeMin();
uint16_t getGPSLatitudeDeg();
uint16_t getGPSLatitudeMin();
uint16_t getGPSSpeed();
uint16_t getGPSHeading();

#endif

