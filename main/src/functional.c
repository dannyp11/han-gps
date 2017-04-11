#include "functional.h"

void iterateChannel(BaseChannel *pChn, void (*action)(msg_t)) {
  msg_t c;
  do {
    c = chnGetTimeout(pChn, TIME_IMMEDIATE);
    if (c != Q_TIMEOUT && c != Q_RESET)
      action(c);
  } while (c != Q_TIMEOUT && c != Q_RESET);
}
