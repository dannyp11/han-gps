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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ch.h"
#include "hal.h"
#include "test.h"

#include "shell.h"
#include "chprintf.h"

#include "usbcfg.h"

/*===========================================================================*/
/* Command line related.                                                     */
/*===========================================================================*/

#define SHELL_WA_SIZE   THD_WORKING_AREA_SIZE(2048)
#define TEST_WA_SIZE    THD_WORKING_AREA_SIZE(256)

static void cmd_mem(BaseSequentialStream *chp, int argc, char *argv[]) {
  size_t n, size;

  (void)argv;
  if (argc > 0) {
    chprintf(chp, "Usage: mem\r\n");
    return;
  }
  n = chHeapStatus(NULL, &size);
  chprintf(chp, "core free memory : %u bytes\r\n", chCoreGetStatusX());
  chprintf(chp, "heap fragments   : %u\r\n", n);
  chprintf(chp, "heap free total  : %u bytes\r\n", size);
}

static void cmd_threads(BaseSequentialStream *chp, int argc, char *argv[]) {
  static const char *states[] = {CH_STATE_NAMES};
  thread_t *tp;

  (void)argv;
  if (argc > 0) {
    chprintf(chp, "Usage: threads\r\n");
    return;
  }
  chprintf(chp, "    addr    stack prio refs     state\r\n");
  tp = chRegFirstThread();
  do {
    chprintf(chp, "%08lx %08lx %4lu %4lu %9s %lu\r\n",
             (uint32_t)tp, (uint32_t)tp->p_ctx.r13,
             (uint32_t)tp->p_prio, (uint32_t)(tp->p_refs - 1),
             states[tp->p_state]);
    tp = chRegNextThread(tp);
  } while (tp != NULL);
}

static void cmd_test(BaseSequentialStream *chp, int argc, char *argv[]) {
  thread_t *tp;

  (void)argv;
  if (argc > 0) {
    chprintf(chp, "Usage: test\r\n");
    return;
  }
  tp = chThdCreateFromHeap(NULL, TEST_WA_SIZE, chThdGetPriorityX(),
                           TestThread, chp);
  if (tp == NULL) {
    chprintf(chp, "out of memory\r\n");
    return;
  }
  chThdWait(tp);
}

#define debug(...) chprintf(chp, __VA_ARGS__)

/*
 * Command to monitor I2C
 */
static uint8_t rx_data[16];
static i2cflags_t errors = 0;
static void cmd_i2c_monitor(BaseSequentialStream *chp, int argc, char *argv[]) {
  msg_t status = MSG_OK;
  systime_t tmo = MS2ST(4);
  uint8_t i;
  uint8_t count;

  count = argc > 4 ? 4 : argc;
  if (argc == 0) {
    chprintf(chp, "Usage: i2c_monitor [addr1] [addr2] [addr3] [addr4]\r\n");
    return;
  }

  for (i = 0; i < 16; ++i) {
    rx_data[i] = 0;
  }


  for (i = 0; i < count; ++i) {
    int i2c_addr;

    i2c_addr = atoi((const char*) argv[i]);
    i2cAcquireBus(&I2CD1);
    status = i2cMasterReceiveTimeout(&I2CD1, i2c_addr, rx_data, 16, tmo);
    i2cReleaseBus(&I2CD1);

    if (status == MSG_RESET){
      errors = i2cGetErrors(&I2CD1);
      chprintf(chp, "Error %d\r\n", errors);
      osalDbgCheck(I2C_ACK_FAILURE == errors);
    }
    else {
      for (i = 0; i < 16; ++i) {
      	chprintf(chp, "0x%02X", rx_data[i]);
      }
      chprintf(chp, "\r\n");
    }
  }
}

static uint8_t pipeByte(BaseSequentialStream *chp1, BaseSequentialStream *chp2) {
  int8_t b;
  b = chSequentialStreamGet(chp1);
  if (b != Q_RESET) {
    chSequentialStreamPut(chp2, b);
  }
  /* ETX.*/
  if ((b ^ 0x03) == 0) {
    return 1;
  }
  return 0;
}

/**
 * @brief This function makes the board act as a TTL-USB dongle
 *
 * @details Invoke with `serial [BAUD]`
 */
static void cmd_serial_console(BaseSequentialStream *chpu, int argc, char *argv[]) {
  int baud;
  SerialConfig sd1cfg = {9600,0,0,0};
  BaseSequentialStream *chp1 = (BaseSequentialStream *) &SD1;

  if (argc == 0) {
    baud = 9600;
  }
  else if (argc == 1) {
    baud = atoi(argv[0]);
  }
  else {
    chprintf(chpu, "Usage: serial [BAUD]\r\n");
    chprintf(chpu, "    PA9 is TX, PA10 is RX.\r\n");
  }
  sd1cfg.speed = baud;
  sdStart(&SD1, &sd1cfg);
  while (1) {
    /* Computer to device. Exit if ^C pressed.*/
    if (pipeByte(chpu, chp1)) break;
    /* Device to computer. Device cannot break communication.*/
    pipeByte(chp1, chpu);
  }
}

static const ShellCommand commands[] = {
  {"mem", cmd_mem},
  {"threads", cmd_threads},
  {"test", cmd_test},
  {"i2c_monitor", cmd_i2c_monitor},
  {"serial", cmd_serial_console},
  {NULL, NULL}
};

static const ShellConfig shell_cfg1 = {
  (BaseSequentialStream *)&SDU1,
  commands
};

/*===========================================================================*/
/* Generic code.                                                             */
/*===========================================================================*/

/*
 * Red LED blinker thread, times are in milliseconds.
 */
static THD_WORKING_AREA(waThread1, 128);
static THD_FUNCTION(Thread1, arg) {

  (void)arg;

  chRegSetThreadName("blinker");
  while (true) {
    systime_t time = serusbcfg.usbp->state == USB_ACTIVE ? 250 : 500;
    palClearPad(IOPORT3, GPIOC_LED);
    chThdSleepMilliseconds(time);
    palSetPad(IOPORT3, GPIOC_LED);
    chThdSleepMilliseconds(time);
  }
}

/*
 * I2C1 config.
 */
static const I2CConfig i2cfg1 = {
    OPMODE_I2C,
    400000,
    FAST_DUTY_CYCLE_2,
};


/*
 * Application entry point.
 */
int main(void) {
  thread_t *shelltp = NULL;

  /*
   * System initializations.
   * - HAL initialization, this also initializes the configured device drivers
   *   and performs the board-specific initializations.
   * - Kernel initialization, the main() function becomes a thread and the
   *   RTOS is active.
   */
  halInit();
  chSysInit();

  i2cStart(&I2CD1, &i2cfg1);

  /*
   * Initializes a serial-over-USB CDC driver.
   */
  sduObjectInit(&SDU1);
  sduStart(&SDU1, &serusbcfg);

  /*
   * Initializes USART1
   */
  sdObjectInit(&SD1, NULL, NULL);

  /*
   * Activates the USB driver and then the USB bus pull-up on D+.
   * Note, a delay is inserted in order to not have to disconnect the cable
   * after a reset.
   */
  usbDisconnectBus(serusbcfg.usbp);
  chThdSleepMilliseconds(1500);
  usbStart(serusbcfg.usbp, &usbcfg);
  usbConnectBus(serusbcfg.usbp);

  /*
   * Shell manager initialization.
   */
  shellInit();

  /*
   * Creates the blinker thread.
   */
  chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO, Thread1, NULL);

  /*
   * Normal main() thread activity, in this demo it does nothing except
   * sleeping in a loop and check the button state.
   */
  while (true) {
    if (!shelltp && (SDU1.config->usbp->state == USB_ACTIVE))
      shelltp = shellCreate(&shell_cfg1, SHELL_WA_SIZE, NORMALPRIO);
    else if (chThdTerminatedX(shelltp)) {
      chThdRelease(shelltp);    /* Recovers memory of the previous shell.   */
      shelltp = NULL;           /* Triggers spawning of a new shell.        */
    }
    chThdSleepMilliseconds(1000);
  }
}
