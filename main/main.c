// chibios includes here
#include "ch.h"
#include "chprintf.h"
#include "debug.h"
#include "hal.h"

// HW includes (avr, lcd, gps, etc.) here
// Note: don't include lcd, led, buttons, encoder,
// 		photocell since they are in UIThread
#include "Compass.h"
#include "computation.h"
#include "gps.h"
#include "xbee.h"
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
#define DRIVERPRIO HIGHPRIO

/*
 * Global variables here, pls don't use static
 * g_ stands for global
 * g_my : this device
 * g_friend : the other device
 */
uint8_t g_myID, g_friendID;
uint8_t g_myLatitude, g_myLongtitude; // change to your type
uint8_t g_friendLatitude, g_friendLongtitude;
CompassDirection g_myCompassDirection, g_friendCompassDirection;
CompassDirection g_friendCardinalDirection;
uint8_t g_myMessageCode, g_friendMessageCode; // send/rcv message code

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
  g_myLatitude = 1;
  g_myLongtitude = 2;
  g_myCompassDirection = NORTH;
  g_myMessageCode = 0;
  g_friendMessageCode = 0;
  g_friendCompassDirection = SOUTH;
  g_friendLatitude = 3;
  g_friendLongtitude = 4;
  g_friendID = 1;

  sdStart(&SD1, NULL);
  info("SD1 Started\r\n");
  sdStart(&SDS, &softserial_config);

  /*
	 * Init all modules here
	 * initialization shouldn't go into thread since it's only called once
	 */
  // gps
  // xbee
  // buzzer

  /*
	 * Run all threads
	 */
  // chThdCreateStatic(waTdUI, sizeof(waTdUI), NORMALPRIO, tdUI, NULL);
  // gps thread
  // xbee thread
  chThdCreateStatic(waTdGPS, sizeof(waTdGPS), NORMALPRIO, tdGPS, NULL);
  chThdCreateStatic(waTdXBee, sizeof(waTdXBee), NORMALPRIO, tdXBee, NULL);
  chThdCreateStatic(waTdComp, sizeof(waTdComp), NORMALPRIO, tdComp, NULL);

  /*
	 * main thread, main logic here
	 * all code that has no delay (such as calculation, ...) should be here
	 */
  while (true) {
    chThdSleepSeconds(1);
  }
}
