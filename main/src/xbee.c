
#include "xbee.h"
#include "ch.h"
#include "functional.h"
#include "xbeeParser.h"

MAILBOX_DECL(xbeeMailbox, xbeeMailboxBuf,sizeof(msg_t) * XBEE_MAILBOX_SIZE);
msg_t xbeeMailboxBuf[XBEE_MAILBOX_SIZE];

MEMORYPOOL_DECL(xbeeMemoryPool, XBEE_MAILBOX_SIZE*sizeof(peer_message_t),NULL);

void xbee_init(void) {
  sdStart(&SD1, NULL);

  // TODO: Initialize memory pool, etc.
  //chMBObjectInit(&xbeeMailbox, xbeeMailboxBuf, XBEE_MAILBOX_SIZE);
  //chPoolObjectInit(xbeeMemoryPool, sizeof(peer_message_t), NULL);
  xbeeSetCallback(&xbeeMailbox, &xbeeMemoryPool);
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