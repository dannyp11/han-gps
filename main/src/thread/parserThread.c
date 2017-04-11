#include "functional.h"
#include "gpsParser.h"
#include "parser.h"
#include "xbeeParser.h"
#include "parserThread.h"
#include "softserialcfg.h"

#include <ctype.h>

MAILBOX_DECL(xbeeMailbox, xbeeMailboxBuf, XBEE_MAILBOX_SIZE);
msg_t xbeeMailboxBuf[XBEE_MAILBOX_SIZE];

MEMORYPOOL_DECL(xbeeMemoryPool, XBEE_FIFO_SIZE * sizeof(peer_message_t), NULL);
peer_message_t xbeeMemoryPoolBuf[XBEE_FIFO_SIZE];

static void gps_init(void) {
  // TODO: DEBUG ONLY
  sdStart(&SD1, NULL);
  sdStart(&SDS, &softserial_config);
}

void xbee_init(void) {
  int8_t i;
  // TODO: Initialize memory pool, etc.
  chMBObjectInit(&xbeeMailbox, xbeeMailboxBuf, XBEE_MAILBOX_SIZE);
  chPoolObjectInit(&xbeeMemoryPool, XBEE_FIFO_SIZE * sizeof(peer_message_t), NULL);
  for (i = 0; i < XBEE_FIFO_SIZE; ++i) {
    chPoolFree(&xbeeMemoryPool, &xbeeMemoryPoolBuf[i]);
  }
  xbeeSetCallback(&xbeeMailbox, &xbeeMemoryPool);
}

THD_WORKING_AREA(waTdParser, PARSER_WA_SIZE);
THD_FUNCTION(tdParser, arg) {
  (void)arg;

  event_listener_t elGPSData;
  event_listener_t elXBeeData;

  /* Initializes GPS.*/
  gps_init();
  /* Initialize XBee.*/
  xbee_init();

  chEvtRegisterMaskWithFlags(pGPSEvt, &elGPSData, EVENT_MASK(1), CHN_INPUT_AVAILABLE);
  chEvtRegisterMaskWithFlags(pGPSEvt, &elGPSData, EVENT_MASK(2), CHN_INPUT_AVAILABLE);

  while (true) {
    eventmask_t ev = chEvtWaitAny(EVENT_MASK(1) | EVENT_MASK(2));
    if (ev == EVENT_MASK(1)) {
      chEvtGetAndClearFlags(&elGPSData);
      iterateChannel(pGPSChn, gpsStepParser);
    } else if (ev == EVENT_MASK(2)) {
      chEvtGetAndClearFlags(&elXBeeData);
      iterateChannel(pXBEEChn, xbeeStepParser);
    }
  }
}
