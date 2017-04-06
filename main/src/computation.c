#include "computation.h"
#include "xbee.h"

THD_WORKING_AREA(waTdComp, COMP_WA_SIZE);
THD_FUNCTION(tdComp, arg) {
  (void)arg;
  info_computation("Spawned\r\n");
  while (true) {
    msg_t p;
    peer_message_t peer;
    // // signed char x;

    /* Receive a message.*/
    chMBFetch(&xbeeMailbox, &p, TIME_INFINITE);
    peer = *((peer_message_t *)p);
    chThdSleepSeconds(1);
    info_computation("Peer ID: %d\r\n", peer.peerID);
    info_computation("Longitude Deg: %D\r\n", peer.longitude.degree);
    info_computation("Longitude Min: %D\r\n", peer.longitude.minute);
    info_computation("Latitude Deg: %D\r\n", peer.latitude.degree);
    info_computation("Latitude Min: %D\r\n", peer.latitude.minute);
    info_computation("Msg ID: %d\r\n", peer.msgID);
    /* Free the message.*/
    chThdSleepSeconds(1);
    chPoolFree(&xbeeMemoryPool, (peer_message_t *)p);
    chThdSleepSeconds(1);
  }
}