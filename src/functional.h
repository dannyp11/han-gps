#ifndef __FUNCTIONAL_H__
#define __FUNCTIONAL_H__

#include "ch.h"

/*#define iterateChannel(pChn, action) \
    { \
    msg_t c; \
    do { \
        c = chnGetTimeout(pChn, TIME_IMMEDIATE);  \
        if (c != Q_TIMEOUT && c != Q_RESET) \
            action(c); \
    } while (c != Q_TIMEOUT && c != Q_RESET); \
    }*/

void iterateChannel(BaseChannel *pChn, void (*action)(msg_t)) {
  msg_t c;
  do {
    c = chnGetTimeout(pChn, TIME_IMMEDIATE);
    if (c != Q_TIMEOUT && c != Q_RESET)
      action(c);
  } while (c != Q_TIMEOUT && c != Q_RESET);
}

#endif