#include "gps.h"

static void gps_init(void) {
  // TODO: DEBUG ONLY
  sdStart(&SD1, NULL);
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

  msg_t c;

  event_listener_t elGPSData;

  /* Initializes GPS.*/
  gps_init();

  chEvtRegisterMaskWithFlags(pGPSEvt, &elGPSData, EVENT_MASK(1), CHN_INPUT_AVAILABLE);


  while (true) {
    chThdSleepMilliseconds(200);
    if (chEvtWaitOne(EVENT_MASK(1))) {
      chEvtGetAndClearFlags(&elGPSData);
      do {
        c = chnGetTimeout(pGPSChn, TIME_IMMEDIATE);
        if (c != Q_TIMEOUT && c != Q_RESET)
          chnPutTimeout(pGPSChn, c, TIME_IMMEDIATE);
      } while (c != Q_TIMEOUT && c != Q_RESET);
    }
  }
}
