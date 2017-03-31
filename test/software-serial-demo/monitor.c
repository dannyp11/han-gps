#include "monitor.h"

#include "debug.h"

THD_WORKING_AREA(waTdMon, MONITOR_WA_SIZE);
THD_FUNCTION(tdMon, arg) {
  while (true) {
    signed char x;
    /* This is blocking.*/
    x = sdGet(&SD1);
    chprintf((BaseSequentialStream *)&SD1, "%c", x);
    /* Yield to other threads.*/
    chThdSleepMilliseconds(2);
  }
}