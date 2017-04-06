#ifndef __GPS_PARSER_H__
#define __GPS_PARSER_H__

#include <inttypes.h>

#include "debug.h"
#include "parser.h"

/*
 * Longitude/Latitude are in dddmm.mmmm format. Therefore, the minutes
 * need to divide by 10000
 */
typedef struct {
  int16_t degree;
  float minute;
} deg_min_t;

extern MATCH_FUNC(DegMin);
extern PARSE_FUNC(DegMin);

#define INVALID_GPS_DATA -1

int16_t getGPSLongitudeDeg(void);
float getGPSLongitudeMin(void);
int16_t getGPSLatitudeDeg(void);
float getGPSLatitudeMin(void);

void gpsStepParser(msg_t token);

#endif
