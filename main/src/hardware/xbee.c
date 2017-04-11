
#include "xbee.h"
#include "ch.h"
#include "functional.h"
#include "xbeeParser.h"

MAILBOX_DECL(xbeeMailbox, xbeeMailboxBuf, XBEE_MAILBOX_SIZE);
msg_t xbeeMailboxBuf[XBEE_MAILBOX_SIZE];

MEMORYPOOL_DECL(xbeeMemoryPool, XBEE_FIFO_SIZE*sizeof(peer_message_t),NULL);
peer_message_t xbeeMemoryPoolBuf[XBEE_FIFO_SIZE];

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

THD_WORKING_AREA(waTdXBee, XBEE_WA_SIZE);
THD_FUNCTION(tdXBee, arg) {
  (void)arg;
  event_listener_t elXBeeData;
  info_xbee("XBee Thread Spawned\r\n");
  xbee_init();

  chEvtRegisterMaskWithFlags(pXBEEEvt, &elXBeeData, EVENT_MASK(1),
                             CHN_INPUT_AVAILABLE);

  while (true) {
    if (chEvtWaitOne(EVENT_MASK(1))) {
      //debug_xbee("XBee Evt\r\n");
      chEvtGetAndClearFlags(&elXBeeData);
      iterateChannel(pXBEEChn, xbeeStepParser);
    }
  }
}