#include "parserThread.h"
#include "ch.h"
#include "functional.h"
#include "gpsParser.h"
#include "parser.h"
#include "softserialcfg.h"
#include "xbeeParser.h"

#include "LCD.h"
#include <ctype.h>

MAILBOX_DECL(xbeeMailbox, xbeeMailboxBuf, XBEE_MAILBOX_SIZE);
msg_t xbeeMailboxBuf[XBEE_MAILBOX_SIZE];

MEMORYPOOL_DECL(xbeeMemoryPool, XBEE_FIFO_SIZE * sizeof(peer_message_t), NULL);
peer_message_t xbeeMemoryPoolBuf[XBEE_FIFO_SIZE];

static void gps_init(void) {
  // // TODO: DEBUG ONLY
  // sdStart(&SD1, NULL);
  // sdStart(&SDS, &softserial_config);
}

void xbee_init(void) {
  int8_t i;
  chMBObjectInit(&xbeeMailbox, xbeeMailboxBuf, XBEE_MAILBOX_SIZE);
  chPoolObjectInit(&xbeeMemoryPool, XBEE_FIFO_SIZE * sizeof(peer_message_t), NULL);
  for (i = 0; i < XBEE_FIFO_SIZE; ++i) {
    chPoolFree(&xbeeMemoryPool, &xbeeMemoryPoolBuf[i]);
  }
  xbeeSetCallback(&xbeeMailbox, &xbeeMemoryPool);
}

THD_WORKING_AREA(waTdParser, PARSER_WA_SIZE);
#if 0
THD_FUNCTION(tdParser, arg) {
  while(1) {chThdSleepMilliseconds(100);}
}
#endif

THD_FUNCTION(tdParser, arg) {
  (void)arg;

  event_listener_t elGPSData;
  event_listener_t elXBeeData;

  /* Initializes GPS.*/
  // gps_init();
  /* Initialize XBee.*/
  xbee_init();
//  LCDInit();


  // chEvtRegisterMaskWithFlags(pGPSEvt, &elGPSData, EVENT_MASK(1), CHN_INPUT_AVAILABLE);
  chEvtRegisterMaskWithFlags(pXBEEEvt, &elXBeeData, EVENT_MASK(2), CHN_INPUT_AVAILABLE);

  while (true) {
    eventmask_t ev = chEvtWaitAny(EVENT_MASK(1) | EVENT_MASK(2));
    // if (ev & EVENT_MASK(1)) {
    //   chEvtGetAndClearFlags(&elGPSData);
    //   iterateChannel(pGPSChn, gpsStepParser);
    // }
    if (ev & EVENT_MASK(2)) {
      static char buffer[27];
      static int8_t state = 0;
      static int8_t i = 0;
      msg_t c;
      chEvtGetAndClearFlags(&elXBeeData);
      c = chnGetTimeout(pXBEEChn, TIME_IMMEDIATE);

//      LCDPrint((char*) &c);

      chnPutTimeout(pXBEEChn, c, TIME_IMMEDIATE);
      /* Preprocess "#...$".*/
      if (c != Q_TIMEOUT && c != Q_RESET) {
        if (state == 0 && c == '#') {
          ++state;
        } else if (state != 0) {
          if (c != '$') {
            /* Valid.*/
            if (isalnum(c) || ispunct(c) || isspace(c)) {
              buffer[i++] = c;
            } else { /* Invalid characters.*/
              goto cleanup;
            }
          } else { /* Parse End($).*/
            buffer[i++] = c;
            goto start_parse;
          }
        } else { /* Parse end or invalid.*/
          int8_t x;
        start_parse:
          for (x = 0; x < i; ++x) {
            xbeeStepParser(buffer[x]);
          }
        cleanup:
          i = 0;
          state = 0;
        }
      }
      // iterateChannel(pXBEEChn, xbeeStepParser);
    }
  }
}
