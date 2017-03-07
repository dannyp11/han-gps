#include "monitor.h"
#include "gps.h"

#include "debug.h"
#include "gpsParser.h"

THD_WORKING_AREA(waTdMon, MONITOR_WA_SIZE);
THD_FUNCTION(tdMon, arg) {
  uint8_t i;
  while (true) {

    char buf[8] = {'G', 'G', 'A', 0, 'Y', 0, 0, 0};
    for (i = 0; i < 5; ++i)
      chThdSleepSeconds(1);

    debug("GPS Longitude Degrees: %d\r\n", getGPSLongitudeDeg());
    debug("GPS Longitude Minutesx10000: %d\r\n", getGPSLongitudeMin());
    debug("GPS Latitude Degrees: %d\r\n", getGPSLatitudeDeg());
    debug("GPS Latitude Minutesx10000: %d\r\n", getGPSLatitudeMin());
  }
}