#pragma once

#include "ch.h"

#define pGPS_SD (SerialDriver *) &SDS
#define pGPSChs (BaseSequentialStream *)pGPS_SD
#define pGPSChn (BaseChannel *)pGPS_SD
#define pGPSEvt (event_source_t *) chnGetEventSource(pGPS_SD)

#define pXBEE_SD (SerialDriver *) &SD1
#define pXBEEChs (BaseSequentialStream *)pXBEE_SD
#define pXBEEChn (BaseChannel *)pXBEE_SD
#define pXBEEEvt (event_source_t *) chnGetEventSource(pXBEE_SD)

/* These are means to exchange information with main algorithm.*/
#define XBEE_MAILBOX_SIZE 4
#define XBEE_FIFO_SIZE 4
extern mailbox_t xbeeMailbox;
extern msg_t xbeeMailboxBuf[XBEE_MAILBOX_SIZE];
extern memory_pool_t xbeeMemoryPool;

#define PARSER_WA_SIZE 160

extern THD_WORKING_AREA(waTdParser, PARSER_WA_SIZE);
extern THD_FUNCTION(tdParser, arg);
