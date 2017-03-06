#ifndef __GPS_PARSER_H__
#define __GPS_PARSER_H__

#include <inttypes.h>

#include "debug.h"
#include "parser.h"

#define INVALID_GPS_DATA (uint16_t)0xFFFF

uint16_t getGPSLongitudeDeg(void);
uint16_t getGPSLongitudeMin(void);
uint16_t getGPSLatitudeDeg(void);
uint16_t getGPSLatitudeMin(void);

void gpsStepParser(msg_t token);

#endif
