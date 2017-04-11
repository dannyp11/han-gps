// chibios includes here
#include "ch.h"
#include "chprintf.h"
#include "debug.h"
#include "hal.h"

// HW includes (avr, lcd, gps, etc.) here
// Note: don't include lcd, led, buttons, encoder,
// 		photocell since they are in UIThread
#include <avr/io.h>
#include "Compass.h"
#include "xbee.h"
#include "gps.h"

// SW includes (modules, algorithm, etc.) here
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "UIThread.h"
#include "softserialcfg.h"

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
 * Threads static table, one entry per thread. The number of entries must
 * match NIL_CFG_NUM_THREADS.
 */

/*THD_TABLE_BEGIN
 THD_TABLE_ENTRY(waTdGPS, "GPS", tdGPS, NULL)
 THD_TABLE_END*/

/*
 * Other helper functions
 * Pls create module if these functions are too big, for structure control purpose
 */
#define UI_WA_SIZE 128
THD_WORKING_AREA(waTdUI, UI_WA_SIZE);
THD_FUNCTION(tdUI, arg)
{
	while (1)
	{
		UILoop();
		g_myCompassDirection = CompassGetDirection();
		chThdSleepMicroseconds(100);
	}
}

/*
 * Main code here
 */
int main(void)
{
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
	UIInit();
	// gps
	// xbee
	// buzzer

	/*
	 * Run all threads
	 */
	chThdCreateStatic(waTdUI, sizeof(waTdUI), NORMALPRIO, tdUI, NULL);
	// gps thread
	// xbee thread
	// chThdCreateStatic(waTdGPS, sizeof(waTdGPS), NORMALPRIO, tdGPS, NULL);
	// chThdCreateStatic(waTdLCD, sizeof(waTdLCD), DRIVERPRIO, tdLCD, NULL);
	// chThdCreateStatic(waTdLED, sizeof(waTdLED), DRIVERPRIO, tdLED, NULL);
	chThdCreateStatic(waTdXBee, sizeof(waTdXBee), NORMALPRIO, tdXBee, NULL);

	/*
	 * main thread, main logic here
	 * all code that has no delay (such as calculation, ...) should be here
	 */
	chThdSleepSeconds(1);
	while (true)
	{
		// get friend's info from xbee
		// calculate friend's cardinal direction
		// calculate friend's distance
		// if get alert message from/to friend, call UIAlertToFriend

		msg_t p;
		peer_message_t peer;
		// // signed char x;

		/* Receive a message.*/
		chMBFetch(&xbeeMailbox, &p, TIME_INFINITE);
		peer = *((peer_message_t*) p);
		chThdSleepSeconds(1);info("Main size=%d p=%d\r\n", sizeof(p), (peer_message_t*)p);info("Main Peer ID: %d\r\n", peer.peerID);info("Main Longitude Degree: %D\r\n", peer.longitude.degree);info("Main Longitude Minute: %D\r\n", peer.longitude.minute);info("Main Latitude Degree: %D\r\n", peer.latitude.degree);info("Main Latitude Minute: %D\r\n", peer.latitude.minute);info("Main Msg ID: %d\r\n", peer.msgID);
		/* Free the message.*/
		chThdSleepSeconds(1);
		chPoolFree(&xbeeMemoryPool, (peer_message_t*) p);

		chThdSleepSeconds(1);
	}
}
