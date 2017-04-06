
#include "xbee.h"
#include "ch.h"
#include "functional.h"
#include "xbeeParser.h"

void xbee_init(void) {
  sdStart(&SD1, NULL);

  // TODO: Initialize memory pool, etc.
}

THD_WORKING_AREA(waTdXBee, XBEE_WA_SIZE);
THD_FUNCTION(tdXBee, arg) {
  (void)arg;
  event_listener_t elXBeeData;
  xbee_init();

  chEvtRegisterMaskWithFlags(pXBEEEvt, &elXBeeData, EVENT_MASK(1),
                             CHN_INPUT_AVAILABLE);

  while (true) {
    if (chEvtWaitOne(EVENT_MASK(1))) {
      chEvtGetAndClearFlags(&elXBeeData);
      iterateChannel(pXBEEChn, xbeeStepParser);
    }
  }
}