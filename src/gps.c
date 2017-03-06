#include "gps.h"
#include "functional.h"
#include "parser.h"
#include "gpsParser.h"

#include <ctype.h>

static void gps_init(void) {
  // TODO: DEBUG ONLY
  sdStart(&SD1, NULL);
  sdStart(&SDS, &softserial_config);
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
