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

#include "hal.h"
#include "ch.h"
#include "chprintf.h"
#include "softserialcfg.h"

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


  //chThdSetPriority(IDLEPRIO);
  //chThdCreateStatic(waTdMon, sizeof(waTdMon), NORMALPRIO, tdMon, NULL);

  while (true) {
    chprintf((BaseSequentialStream *) &SD1, "USART1!\r\n");
    //chprintf((BaseSequentialStream *) &SDS, "USARTS!\r\n");
    sdPut(&SDS, 'S');
    chThdSleepSeconds(1);
  }
}