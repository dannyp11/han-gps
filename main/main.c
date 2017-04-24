// chibios includes here
#include "ch.h"
#include "chprintf.h"
#include "debug.h"
#include "hal.h"

// HW includes (avr, lcd, gps, etc.) here
// Note: don't include lcd, led, buttons, encoder,
// 		photocell since they are in UIThread
#include "computationThread.h"
#include "parserThread.h"
#include <avr/io.h>

// SW includes (modules, algorithm, etc.) here
#include "UIThread.h"
#include "softserialcfg.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

/*
 * ChibiOS config
 */
#define INTERACTIVEPRIO HIGHPRIO

/*
 * Global variables here, pls don't use static
 * g_ stands for global
 * g_my : this device
 * g_friend : the other device
 */
uint8_t g_myID;                      // only main can write
float g_myCompassAngle;              // only UI can write
// alert_message_t g_alerts[MAX_PEERS]; // only computation can write

/*
 * Main code here
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

  /*
	 * Inits all global variables here
	 */
  g_myID = 0;
  g_myCompassAngle = 180.0f;

<<<<<<< HEAD
  sdStart(&SD1, NULL);
  sdStart(&SDS, &softserial_config);
  info("SDS Started A\r\n");
=======
	sdStart(&SD1, NULL);
	sdStart(&SDS, &softserial_config);
	info("SD1 Started\r\n"); info("SDS Started\r\n");
>>>>>>> dat_trunk

  /*
	 * Init all modules here
	 * initialization shouldn't go into thread since it's only called once
	 */
  UIInit();

  /*
	 * Run all threads
	 */
<<<<<<< HEAD
  chThdCreateStatic(waTdUI, sizeof(waTdUI), INTERACTIVEPRIO, tdUI, NULL);
  chThdCreateStatic(waTdParser, sizeof(waTdParser), NORMALPRIO, tdParser, NULL);
  chThdCreateStatic(waTdComp, sizeof(waTdComp), NORMALPRIO, tdComp, NULL);
=======
	chThdCreateStatic(waTdUI, sizeof(waTdUI), INTERACTIVEPRIO, tdUI, NULL);
	chThdCreateStatic(waTdParser, sizeof(waTdParser), NORMALPRIO, tdParser, NULL);
	chThdCreateStatic(waTdComp, sizeof(waTdComp), NORMALPRIO, tdComp, NULL);
>>>>>>> dat_trunk

  /*
	 * main thread, main logic here
	 * all code that has no delay (such as calculation, ...) should be here
	 */

<<<<<<< HEAD
  while (true) {
    static int i = 0;
    info("Alive %d\r\n", i);
    ++i;
    chThdSleepSeconds(1);
  }
=======
	while (true)
	{
//		UISendMessage(111.2345f, 123.4567f, 1);
		chThdSleepSeconds(1);
	}
>>>>>>> dat_trunk
}
