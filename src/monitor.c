#include "monitor.h"
#include "gps.h"

#define debug(...) chprintf((BaseSequentialStream *)&SD1, __VA_ARGS__)

THD_WORKING_AREA(waTdMon, MONITOR_WA_SIZE);
THD_FUNCTION(tdMon, arg) {
    uint8_t i;
    while (true) {
        chThdSleepSeconds(5);
        debug("GPS Longitude Degrees: %d", getGPSLongitudeDeg());
        debug("GPS Longitude Minutesx10000: %d", getGPSLongitudeMin());
        debug("GPS Latitude Degrees: %d", getGPSLatitudeDeg());
        debug("GPS Latitude Minutesx10000: %d", getGPSLatitudeMin());
        debug("GPS Stack: ");
        for (i = 0; i < 128; ++i) {
            debug("<0x%x>", waTdGPS[i]);
        }
    }
}