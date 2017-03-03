#include "gps.h"

static thread_t *tp_GPS;
/**
 * @brief Callback function of SDS which notifies GPS thread 
 */
static void gpsNotify(io_queue_t *qp) {
  (void) qp;
  chEvtSignal(tp_GPS, EVENT_MASK(1));
}

static void gps_init(void) {
  sdObjectInit(&SDS, gpsNotify, NULL);
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

  /* Get the thread pointer.*/
  tp_GPS = chThdGetSelfX();

  /* Initializes GPS.*/
  gps_init();

  chprintf(chs, "Testing Serial: Echos: \r\n >");

  while (true) {
    if (chEvtWaitAnyTimeout(EVENT_MASK(1), TIME_IMMEDIATE)) {
      while (chnRead((BaseChannel *)&SDS, &c, 1)) {
        chnWrite((BaseChannel *)&SDS, &c, 1);
      }
    }
  }
}
