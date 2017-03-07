#include "monitor.h"
#include "gps.h"

#include "debug.h"
#include "gpsParser.h"

THD_WORKING_AREA(waTdMon, MONITOR_WA_SIZE);
THD_FUNCTION(tdMon, arg) {
  uint8_t i;
  while (true) {
    for (i = 0; i < 5; ++i)
      chThdSleepSeconds(1);

    debug("GPS Latitude Degrees: %D\r\n", getGPSLatitudeDeg());
    debug("GPS Latitude Minutesx10000: %D\r\n", getGPSLatitudeMin());
    debug("GPS Longitude Degrees: %D\r\n", getGPSLongitudeDeg());
    debug("GPS Longitude Minutesx10000: %D\r\n", getGPSLongitudeMin());
  }
}