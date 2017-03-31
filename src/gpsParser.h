#ifndef __GPS_PARSER_H__
#define __GPS_PARSER_H__

#include <inttypes.h>

#include "debug.h"
#include "parser.h"

#define INVALID_GPS_DATA -1

int32_t getGPSLongitudeDeg(void);
int32_t getGPSLongitudeMin(void);
int32_t getGPSLatitudeDeg(void);
int32_t getGPSLatitudeMin(void);

void gpsStepParser(msg_t token);

#endif
