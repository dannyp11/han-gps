#pragma once

#include "ch.h"
#include "hal.h"
#include "xbeeParser.h"

/* These are means to exchange information with main algorithm.*/
#define XBEE_MAILBOX_SIZE 4
#define XBEE_FIFO_SIZE 4
extern mailbox_t xbeeMailbox;
extern msg_t xbeeMailboxBuf[XBEE_MAILBOX_SIZE];
extern memory_pool_t xbeeMemoryPool;

#if DEBUG_XBEE || DEBUG_PARSER
#define XBEE_WA_SIZE 128
#else 
#define XBEE_WA_SIZE 128
#endif
extern THD_WORKING_AREA(waTdXBee, XBEE_WA_SIZE);
extern THD_FUNCTION(tdXBee, arg);

#define pXBEE_SD (SerialDriver *) &SD1
#define pXBEEChs (BaseSequentialStream *)pXBEE_SD
#define pXBEEChn (BaseChannel *)pXBEE_SD
#define pXBEEEvt (event_source_t *) chnGetEventSource(pXBEE_SD)