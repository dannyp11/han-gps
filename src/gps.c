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

#define PARSER_NUM 6

PARSER_MATCH_FUNC(GGA) {
  if (i < 2 && c == 'G') {
    return MATCH_PARTIAL;
  } else if (i == 3 && c == 'A') {
    return MATCH_SUCCESS;
  }
  return MATCH_FAILED;
}

/**
 * @brief Declares the parser match rules.
 */
/*match_result_t (*const gpsMatchFuncTable[PARSER_NUM])(msg_t, uint8_t) = {*/
const match_func_t gpsMatchFuncTable[PARSER_NUM] = {
    /* Begin: $.*/
    match_Dollar,
    /* ID: aa.*/
    match_UpperCase,
    /* Fix Data: GGA.*/
    match_GGA,
    /* Section Break.*/
    match_Comma};

void gpsRunParserOnce(msg_t c) {
  /* Which parser is being used.*/
  static uint8_t parserState = 0;
  /* Counter for individual parser.*/
  static uint8_t i = 0;
  /* Stores partial match result.*/
  static msg_t buf[16];
  /* The current parser.*/
  match_result_t (*match)(msg_t, uint8_t) = gpsMatchFuncTable[parserState];
  /* Parse result.*/
  match_result_t match_result = match(c, i);
  switch (match_result) {
  case MATCH_PARTIAL:
    /* Increment counter current matcher.*/
    ++i;
    break;
  case MATCH_SUCCESS:
    /* Terminate string if necessary.*/
    if (++i < 16) buf[i] = '\0';
    /* Reset counter for next matcher.*/
    i = 0;
    /* Move to next matcher.*/
    ++parserState;
    break;
  /* Everything unexpected is treated as failure.*/
  default:
    /* Reset counter for next matcher.*/
    i = 0;
    /* Start over.*/
    parserState = 0;
    break;
  }
}

void parseFailCleanUp(void) {
  longitudeDeg = INVALID_GPS_DATA;
  longitudeMin = INVALID_GPS_DATA;
  latitudeDeg = INVALID_GPS_DATA;
  latitudeMin = INVALID_GPS_DATA;
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

THD_WORKING_AREA(waTdGPS, 128);
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
      iterateChannel(pGPSChn, testAction);
    }
  }
}
