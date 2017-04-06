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
  int32_t degree;
  int32_t minute;
} deg_min_t;

extern MATCH_FUNC(DegMin);
extern PARSE_FUNC(DegMin);

#define INVALID_GPS_DATA -1

int32_t getGPSLongitudeDeg(void);
int32_t getGPSLongitudeMin(void);
int32_t getGPSLatitudeDeg(void);
int32_t getGPSLatitudeMin(void);

void gpsStepParser(msg_t token);

#endif
