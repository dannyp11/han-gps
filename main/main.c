/*
    ChibiOS - Copyright (C) 2006..2015 Giovanni Di Sirio

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

#include "ch.h"
#include "chprintf.h"
#include "debug.h"
#include "gps.h"
#include "hal.h"
#include "lcd.h"
#include "led.h"
#include "softserialcfg.h"
#include "xbee.h"

#define DRIVERPRIO HIGHPRIO

/*
 * Threads static table, one entry per thread. The number of entries must
 * match NIL_CFG_NUM_THREADS.
 */
/*THD_TABLE_BEGIN
  THD_TABLE_ENTRY(waTdGPS, "GPS", tdGPS, NULL)
THD_TABLE_END*/

/*
 * Application entry point.
 */
int main(void) {

  /*
   * System initializations.
   * - HAL initialization, this also initializes the configured device drivers
   *   and performs the board-specific initializations.
   * - Kernel initialization, the main() function becomes a thread and the
   *   RTOS is active.
   */
  halInit();
  chSysInit();

  sdStart(&SD1, NULL);
  sdStart(&SDS, &softserial_config);

  // chThdCreateStatic(waTdGPS, sizeof(waTdGPS), NORMALPRIO, tdGPS, NULL);
  // chThdCreateStatic(waTdLCD, sizeof(waTdLCD), DRIVERPRIO, tdLCD, NULL);
  // chThdCreateStatic(waTdLED, sizeof(waTdLED), DRIVERPRIO, tdLED, NULL);
  chThdCreateStatic(waTdXBee, sizeof(waTdXBee), NORMALPRIO, tdXBee, NULL);

  while (true) {
    peer_message_t *p;
    peer_message_t peer;
    // signed char x;

    /* Receive a message.*/
    chMBFetch(&xbeeMailbox, (msg_t *)p, TIME_INFINITE);
    /* Copy it.*/
    peer = *p;
    /* Free the message.*/
    chPoolFree(&xbeeMemoryPool, p);
    debug("XBee Peer ID: %d\r\n", peer.peerID);
    debug("XBee Longitude Degree: %D\r\n", peer.longitude.degree);
    debug("XBee Longitude Minute: %D\r\n", peer.longitude.minute);
    debug("XBee Latitude Degree: %D\r\n", peer.latitude.degree);
    debug("XBee Latitude Minute: %D\r\n", peer.latitude.minute);
    debug("XBee Msg ID: %d\r\n", peer.msgID);

    //chThdSleepSeconds(1);
  }
}