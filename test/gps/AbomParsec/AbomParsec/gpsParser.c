#include "gpsParser.h"
#include "parser.h"

#include <ctype.h>
#include <stdlib.h>
#include <math.h>

/*
 * Longitude/Latitude are in dddmm.mmmm format. Therefore, the minutes
 * need to divide by 10000
 */
typedef struct {
  int32_t degree;
  int32_t minute;
} deg_min_t;

static deg_min_t longitude = {INVALID_GPS_DATA, INVALID_GPS_DATA};
static deg_min_t latitude = {INVALID_GPS_DATA, INVALID_GPS_DATA};

MATCH_FUNC(GGA) {
  if (i < 2 && c == 'G') {
    return MATCH_PARTIAL;
  } else if (i == 2 && c == 'A') {
    return MATCH_SUCCESS;
  }
  return MATCH_FAILED;
}

/**
 * @brief Matches ID. Currently ignored.
 */
MATCH_ANY(ID, 2);

/**
 * @brief Matches UTC time. Currently ignored.
 */
MATCH_UNTIL(Time, ',', 11);

/**
 * @brief Matcher for deg_min. Multiple decimals are not handled.
 */
MATCH_FUNC(DegMin) {
  if (i < 11 && (isdigit(c) || c == '.' || c == ',')) {
    return c == ',' ? MATCH_SUCCESS : MATCH_PARTIAL;
  }
  return MATCH_FAILED;
}

/**
 * @brief Matches all the way up tp CRLF. Split into two parts to avoid buffer overflow
 */
MATCH_ANY(Rest1, 15);
MATCH_ANY(Rest2, 15);

/**
 * @brief Parsing degree_minute section 1. Terrible, terrible floating points!
 */
PARSE_FUNC(DegMin) {
  deg_min_t *p = write_back;
  float degreeF;
  int32_t deg;
  /* Remove the comma.*/
  buf[length - 1] = '\0';

  //debug("[Parse_DegMin] buf=%s, length=%d, degreeF=%.3f, degree=%d, sizeof(degree)=%d\r\n", buf, length, degreeF, deg, sizeof(deg));
  //debug("[Parse_DegMin] sizeof(degree)=%d\r\n", sizeof(deg));
  degreeF = atof(buf) * 10000.f;
  deg = degreeF;  
  //debug("[Parse_DegMin] buf=%s, length=%d, degreeF=%.3f, degree=%D\r\n", buf, length, degreeF, deg);
  //debug("[Parse_DegMin] degree=%D\r\n", deg);
  p->minute = (deg % 1000000L);
  p->degree = (deg / 1000000L);
  return PARSE_SUCCESS;
}

void gpsParseCleanup(void) {
  longitude.degree = INVALID_GPS_DATA;
  longitude.minute = INVALID_GPS_DATA;
  latitude.degree = INVALID_GPS_DATA;
  latitude.minute = INVALID_GPS_DATA;
}

parser_t gpsParser(parserstate_t parserState) {
  switch (parserState) {
    case 0: return new_parser(match_Dollar, NULL, NULL);
    case 1: return new_parser(match_ID, NULL, NULL);
    case 2: return new_parser(match_GGA, NULL, NULL);
    case 3: return new_parser(match_Comma, NULL, NULL);
    case 4: return new_parser(match_Time, NULL, NULL);
    case 5: return new_parser(match_DegMin, parse_DegMin, (writeback_t)&latitude);
    case 6: return new_parser(match_UpperCase, NULL, NULL);
    case 7: return new_parser(match_Comma, NULL, NULL);
    case 8: return new_parser(match_DegMin, parse_DegMin, (writeback_t)&longitude);
    case 9: return new_parser(match_UpperCase, NULL, NULL);
    case 10: return new_parser(match_Comma, NULL, NULL);
    case 11: return new_parser(match_Rest1, NULL, NULL);
    case 12: return new_parser(match_Rest2, NULL, NULL);
    case 13: return new_parser(match_LF, NULL, NULL);
    default: return new_parser(NULL, NULL, NULL);
  }
}

static char gpsBuf[16];
static parserstate_t gpsParserState = 0;
static parserstate_t gpsCnt = 0;
void gpsStepParser(msg_t c) {
  stepParser(c, gpsParser, gpsParseCleanup, gpsBuf, 16, &gpsParserState, &gpsCnt);
}

int32_t getGPSLongitudeDeg() {
  return longitude.degree;
}

int32_t getGPSLongitudeMin() {
  return longitude.minute;
}

int32_t getGPSLatitudeDeg() {
  return latitude.degree;
}

int32_t getGPSLatitudeMin() {
  return latitude.minute;
}