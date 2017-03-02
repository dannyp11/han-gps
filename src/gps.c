#include "gps.h"

void gps_init() { sdStart(&SDS, &softserial_config); }

/*
 * Longitude/Latitude are in dddmm.mmmm format. Therefore, the minutes 
 * need to divide by 10000 
 */
uint16_t longitudeDeg;
uint16_t longitudeMin;
uint16_t latitudeDeg;
uint16_t latitudeMin;

THD_WORKING_AREA(waTdGPS, 128);
THD_FUNCTION(td_gps, arg) {
  (void)arg;

  gps_init();
  while (true) {
    chprintf((BaseSequentialStream *)&SDS, "Hello World Software Serial! Now "
                                           "type something within the next 2 "
                                           "seconds: ");
    n = sdAsynchronousRead(&SDS, &c, 1);
    /* If there is something read, echo.*/
    if (n == 1) {
      sdAsynchronousWrite(&SDS, &c, 1);
    }
    chThdSleepMilliseconds(2000);
    chprintf((BaseSequentialStream *)&SDS, "\r\n");
    chThdSleepMilliseconds(2000);
  }
}
