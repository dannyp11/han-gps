#include "gpsParser.h"
#include "parser.h"

#include <ctype.h>
#include <stdlib.h>

/*
 * Longitude/Latitude are in dddmm.mmmm format. Therefore, the minutes
 * need to divide by 10000
 */
typedef struct {
  uint16_t degree;
  uint16_t minute;
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
MATCH_UNTIL(Time, ',');

/**
 * @brief Matcher for deg_min. Multiple decimals are not handled.
 */
MATCH_FUNC(DegMin) {
  if (isdigit(c) || c == '.' || c == ',') {
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
  /* Remove the comma.*/
  buf[length - 1] = '\0';
  uint32_t degree = (uint32_t)(atof((char *)buf) * 1000.f);
  p->minute = (uint16_t)(degree % 100000);
  p->degree = (uint16_t)(degree / 100000);
  return PARSE_SUCCESS;
}

const parser_t gpsParserTable[] = {
    {match_Dollar, NULL, NULL},
    {match_ID, NULL, NULL},
    {match_GGA, NULL, NULL},
    {match_Comma, NULL, NULL},
    {match_Time, NULL, NULL},
    {match_DegMin, parse_DegMin, (writeback_t)&latitude},
    {match_UpperCase, NULL, NULL},
    {match_Comma, NULL, NULL},
    {match_DegMin, parse_DegMin, (writeback_t)&longitude},
    {match_UpperCase, NULL, NULL},
    {match_Comma, NULL, NULL},
    {match_Rest1, NULL, NULL},
    {match_Rest2, NULL, NULL},
    {match_LF, NULL, NULL}};

const size_t GPS_PARSER_SIZE = sizeof(gpsParserTable) / sizeof(gpsParserTable[0]);

void gpsParseCleanup(void) {
  longitude.degree = INVALID_GPS_DATA;
  longitude.minute = INVALID_GPS_DATA;
  latitude.degree = INVALID_GPS_DATA;
  latitude.minute = INVALID_GPS_DATA;
}

void gpsStepParser(msg_t c) {
  stepParser(c, GPS_PARSER_SIZE, gpsParserTable, gpsParseCleanup);
}

void gpsStreamParser(msg_t token) {
}

uint16_t getGPSLongitudeDeg() {
  return longitude.degree;
}

uint16_t getGPSLongitudeMin() {
  return longitude.minute;
}

uint16_t getGPSLatitudeDeg() {
  return latitude.degree;
}

uint16_t getGPSLatitudeMin() {
  return latitude.minute;
}