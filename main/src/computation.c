#include "computation.h"
#include "gpsParser.h"
#include "xbee.h"

#include <math.h>

void compute(const peer_message_t *p) {
}

THD_WORKING_AREA(waTdComp, COMP_WA_SIZE);
THD_FUNCTION(tdComp, arg) {
  (void)arg;
  info_computation("Spawned\r\n");
  while (true) {
    msg_t result;
    msg_t p;
    peer_message_t peer;
    // // signed char x;

    /* Receive a message.*/
    result = chMBFetch(&xbeeMailbox, &p, TIME_INFINITE);
    if (result == MSG_OK) {
      /* Copy the message.*/
      peer = *((peer_message_t *)p);
      /* Free the message.*/
      chPoolFree(&xbeeMemoryPool, (peer_message_t *)p);
      {
        float degree = truncf(peer.longitude * 180.f / M_PI);
        float minute = (peer.longitude - (degree * M_PI / 180.f)) * 10800.f / M_PI;
        info_computation("Peer ID: %d\r\n", peer.peerID);
        info_computation("Longitude Radian: %.6f\r\n", peer.longitude);
        info_computation("Longitude Deg: %.f\r\n", degree);
        info_computation("Longitude Min: %.3f\r\n", minute);
        degree = truncf(peer.latitude * 180.f / M_PI);
        minute = (peer.latitude - (degree * M_PI / 180.f)) * 10800.f / M_PI;
        info_computation("Latitude Radian: %.6f\r\n", peer.latitude);
        info_computation("Latitude Deg: %.f\r\n", degree);
        info_computation("Latitude Min: %.3f\r\n", minute);
        info_computation("Msg ID: %d\r\n", peer.msgID);
      }
    }

    /* Update everything.*/
    // compute(peer);

    /* TODO: Debug only.*/
    chThdSleepSeconds(1);
  }
}