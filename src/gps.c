#include "gps.h"
#include "functional.h"
#include "parser.h"

static void gps_init(void) {
  // TODO: DEBUG ONLY
  sdStart(&SD1, NULL);
  sdStart(&SDS, &softserial_config);
}

/*
 * Longitude/Latitude are in dddmm.mmmm format. Therefore, the minutes
 * need to divide by 10000
 */
static uint16_t longitudeDeg = INVALID_GPS_DATA;
static uint16_t longitudeMin = INVALID_GPS_DATA;
static uint16_t latitudeDeg = INVALID_GPS_DATA;
static uint16_t latitudeMin = INVALID_GPS_DATA;

#define GPS_PARSER_SIZE 19

MATCH_FUNC(GGA) {
  if (i < 2 && c == 'G') {
    return MATCH_PARTIAL;
  } else if (i == 3 && c == 'A') {
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
MATCH_ANY(Time, 10);

/**
 * @brief Matcher for Degrees
 */
MATCH_FUNC(Deg) {
  if (i < 4 && isdigit(c)) {
    return i == 4 ? MATCH_SUCCESS : MATCH_PARTIAL;
  }
  return MATCH_FAILED;
}

/**
 * @brief Matcher for Minutes
 */
MATCH_FUNC(Min) {
  if (i <= 1 && isdigit(c))
    return MATCH_PARTIAL;
  else if (i == 2 && c == '.')
    return MATCH_PARTIAL;
  else if (i <= 6 && isdigit(c))
    return i == 6 ? MATCH_SUCCESS : MATCH_PARTIAL;
  return MATCH_FAILED;
}


/**
 * @brief Matches all the way up tp CRLF.
 */
 MATCH_ANY(Rest, 30);

/**
 * @brief Parsing degree is just uint16
 */
#define parse_Deg parse_uint16

/**
 * @brief Parse minute
 */
PARSE_FUNC(Min) {
  msg_t *p = buf;
  uint8_t i;
  for (i = 0; i < length; ++i) {
    if (buf[i] != '.')  {
      *p++ = buf[i];
    }
  }
  *p = '\0';
  return parse_uint16(buf, 0, write_back);
}

/**
 * @brief Declares the parser match rules.
 */
/*match_result_t (*const gpsMatchFuncTable[GPS_PARSER_SIZE])(msg_t, uint8_t) = {*/
const match_func_t gpsMatchFuncTable[GPS_PARSER_SIZE] = {
    /* Begin: $.*/
    match_Dollar,
    /* ID: aa.*/
    match_ID,
    match_Comma,
    /* Fix Data: GGA.*/
    match_GGA,
    match_Comma,
    /* Time: hhmmss.ss.*/
    match_Time,
    match_Comma,
    /* Latitude: dddmm.mmmm.*/
    match_Deg,
    match_Min,
    match_Comma,
    match_UpperCase,
    match_Comma,
    /* Longitude: dddmm.mmmm.*/
    match_Deg,
    match_Min,
    match_Comma,
    match_UpperCase,
    /* Everything else.*/
    match_Rest,
    match_CR,
    match_LF
    };

/**
 * @brief Parser function table
 */
 const parse_func_t gpsParseFuncTable[GPS_PARSER_SIZE] = {
   NULL,
   NULL,
   NULL,
   NULL,
   NULL,
   NULL,
   NULL,
   parse_Deg,
   parse_Min,
   NULL,
   NULL,
   NULL,
   parse_Deg,
   parse_Min,
   NULL,
   NULL,
   NULL,
   NULL,
   NULL
 };

 /**
  * @brief Writeback variables
  */
const writeback_t gpsWritebackTable[GPS_PARSER_SIZE] = {
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  (writeback_t) &latitudeDeg,
  (writeback_t) &latitudeMin,
  NULL,
  NULL,
  NULL,
  (writeback_t) &longitudeDeg,
  (writeback_t) &longitudeMin,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL
};

void gpsParseCleanup(void) {
  longitudeDeg = INVALID_GPS_DATA;
  longitudeMin = INVALID_GPS_DATA;
  latitudeDeg = INVALID_GPS_DATA;
  latitudeMin = INVALID_GPS_DATA;
}

void gpsStepParser(msg_t c) {
  stepParser(c, GPS_PARSER_SIZE, gpsMatchFuncTable, gpsParseFuncTable, gpsWritebackTable, gpsParseCleanup);
}



void gpsStreamParser(msg_t token) {
}

uint16_t getGPSLongitudeDeg() {
  return longitudeDeg;
}

uint16_t getGPSLongitudeMin() {
  return longitudeMin;
}

uint16_t getGPSLatitudeDeg() {
  return latitudeDeg;
}

uint16_t getGPSLatitudeMin() {
  return latitudeMin;
}

static inline void testAction(msg_t c) {
  chnPutTimeout(pGPSChn, c, TIME_IMMEDIATE);
}

THD_WORKING_AREA(waTdGPS, GPS_WA_SIZE);
THD_FUNCTION(tdGPS, arg) {
  (void)arg;

  event_listener_t elGPSData;

  /* Initializes GPS.*/
  gps_init();

  chEvtRegisterMaskWithFlags(pGPSEvt, &elGPSData, EVENT_MASK(1), CHN_INPUT_AVAILABLE);

  while (true) {
    chThdSleepMilliseconds(200);
    if (chEvtWaitOne(EVENT_MASK(1))) {
      chEvtGetAndClearFlags(&elGPSData);
      iterateChannel(pGPSChn, gpsStepParser);
    }
  }
}
