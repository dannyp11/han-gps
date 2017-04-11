#ifndef __GPS_PARSER_H__
#define __GPS_PARSER_H__

#include <inttypes.h>

#include "debug.h"
#include "parser.h"

/*
 * Longitude/Latitude are in dddmm.mmmm format. Therefore, the minutes
 * need to divide by 10000
 */
 typedef float deg_min_t;
// typedef struct {
//   int16_t degree;
//   float minute;
// } deg_min_t;

extern MATCH_FUNC(DegMin);
extern PARSE_FUNC(DegMin);

#define INVALID_GPS_DATA -1

float getGPSLongitude(void);
float getGPSLatitude(void);

void gpsStepParser(msg_t token);

#endif
