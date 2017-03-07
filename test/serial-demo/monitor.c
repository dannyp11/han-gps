#include "monitor.h"

#include "debug.h"
#include "softserialcfg.h"

THD_WORKING_AREA(waTdMon, MONITOR_WA_SIZE);
THD_FUNCTION(tdMon, arg) {
  uint8_t i;
  char c;

  sdStart(&SD1, NULL);
  sdStart(&SDS, &softserial_config);

  while (true) {
    for (i = 0; i < 5; ++i)
      chThdSleepSeconds(1);
    chprintf((BaseSequentialStream *) &SD1, "This is a message from USART1!\r\n");
    chprintf((BaseSequentialStream *) &SDS, "This is a message from Software Serial!\r\n");
    chprintf((BaseSequentialStream *) &SD1, "Now press a key on software serial: ");
    chprintf((BaseSequentialStream *) &SDS, "Now press a key on software serial: ");
    c = chnGetTimeout((BaseChannel *)&SDS, TIME_INFINITE);
    chprintf((BaseSequentialStream *) &SD1, "%c", c);
    chprintf((BaseSequentialStream *) &SDS, "%c", c);
  }
}