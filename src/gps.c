#include "gps.h"

static void gps_init(void) {
  sdStart(&SDS, &softserial_config);
}

/*
 * Longitude/Latitude are in dddmm.mmmm format. Therefore, the minutes
 * need to divide by 10000
 */
uint16_t longitudeDeg;
uint16_t longitudeMin;
uint16_t latitudeDeg;
uint16_t latitudeMin;

THD_WORKING_AREA(waTdGPS, 128);
THD_FUNCTION(tdGPS, arg) {
  (void)arg;

  BaseSequentialStream *chs = (BaseSequentialStream *) &SDS;
  unsigned char c;

  event_listener_t elGPSData;

  /* Initializes GPS.*/
  gps_init();

  chEvtRegisterMaskWithFlags((event_source_t *)chnGetEventSource(&SDS), &elGPSData, EVENT_MASK(1), CHN_INPUT_AVAILABLE);

  chprintf(chs, "Testing Serial: Echos: \r\n >");

  while (true) {
    if (chEvtWaitOne(EVENT_MASK(1))) {
      chEvtGetAndClearFlags(&elGPSData);
        do {
          c = chnGetTimeout((BaseChannel *)&SDS, TIME_IMMEDIATE);
          chnPutTimeout((BaseChannel *)&SDS, c, TIME_INFINITE);
        } while (c != Q_TIMEOUT);
      }
  }
}
