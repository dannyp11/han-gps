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

/**
 * @file    softserial_lld.c
 * @brief   AVR low level serial driver code.
 *
 * @addtogroup SERIAL
 * @{
 */

#include "hal.h"

#if HAL_USE_SERIAL || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

#if AVR_SERIAL_USE_USARTS || defined (__DOXYGEN__)
  #ifndef AVR_GPT_USE_TIM2
    #error "Software serial requires AVR_GPT_USE_TIM2"
  #endif
  /* Uses PCINT0*/
  #ifdef AVR_SDS_USE_PCINT0
    #define AVR_SDS_RX_PORT IOPORT2
    #define AVR_SDS_RX_PIN 0
    #define AVR_SDS_RX_VECT PCINT0_vect
    #define AVR_SDS_RX_TCCR2B_CLK_MASK 0b00000111
  #endif
  /* By default, uses PB1 as TX.*/
  #ifndef AVR_SDS_TX_PORT
    #define AVR_SDS_TX_PORT IOPORT2
  #endif
  #ifndef AVR_SDS_TX_PIN
    #define AVR_SDS_TX_PORT 1
  #endif
#endif

#if 0

/**
 * @brief   USART0 serial driver identifier.
 * @note    The name does not follow the convention used in the other ports
 *          (COMn) because a name conflict with the AVR headers.
 */
#if AVR_SERIAL_USE_USART0 || defined(__DOXYGEN__)
SerialDriver SD1;

  /* USARTs are not consistently named across the AVR range */
  #ifdef USART0_RX_vect
    #define AVR_SD1_RX_VECT USART0_RX_vect
    #define AVR_SD1_TX_VECT USART0_UDRE_vect
  #elif defined(USART_RX_vect)
    #define AVR_SD1_RX_VECT USART_RX_vect
    #define AVR_SD1_TX_VECT USART_UDRE_vect
  #else
    #error "Cannot find USART to use for SD1"
  #endif
#endif /* AVR_SERIAL_USE_USART0 */

/**
 * @brief   USART1 serial driver identifier.
 * @note    The name does not follow the convention used in the other ports
 *          (COMn) because a name conflict with the AVR headers.
 */
#if AVR_SERIAL_USE_USART1 || defined(__DOXYGEN__)
SerialDriver SD2;

  /* Check if USART1 exists for this MCU */
  #ifdef USART1_RX_vect
    #define AVR_SD2_RX_VECT USART1_RX_vect
    #define AVR_SD2_TX_VECT USART1_UDRE_vect
  #else
    #error "Cannot find USART to use for SD2"
  #endif
#endif /* AVR_SERIAL_USE_USART1 */

#endif

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/**
 * @brief   Driver default configuration. Baud 9600 on 7.3728MHz clock
 */
static const SerialConfig default_config = {
  96,
  /* CLK/8.*/
  (1 << CA21),
  8
};

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

static void set_error(uint8_t sra, SerialDriver *sdp) {
  eventflags_t sts = 0;
  uint8_t dor = 0;
  uint8_t upe = 0;
  uint8_t fe = 0;

#if 0
#if AVR_SERIAL_USE_USART0
  if (&SD1 == sdp) {
    dor = (1 << DOR0);
    upe = (1 << UPE0);
    fe = (1 << FE0);
  }
#endif

#if AVR_SERIAL_USE_USART1
  if (&SD2 == sdp) {
    dor = (1 << DOR1);
    upe = (1 << UPE1);
    fe = (1 << FE1);
  }
#endif
#endif
  if (sra & dor)
    sts |= SD_OVERRUN_ERROR;
  if (sra & upe)
    sts |= SD_PARITY_ERROR;
  if (sra & fe)
    sts |= SD_FRAMING_ERROR;
  osalSysLockFromISR();
  chnAddFlagsI(sdp, sts);
  osalSysUnlockFromISR();
}


#if AVR_SERIAL_USE_USARTS || defined(__DOXYGEN__)
static void notifyS(io_queue_t *qp) {
  // TODO
  (void)qp;
  //UCSR0B |= (1 << UDRIE0);
}

/**
 * @brief   USARTS initialization.
 *
 * @param[in] config    the architecture-dependent serial driver configuration
 */
static void usartS_init(const SerialConfig *config) {
  /* Sets appropriate I/O mode.*/
  palSetPadMode(AVR_SDS_RX_PORT, AVR_SDS_RX_PIN, PAL_MODE_INPUT);
  palSetPadMode(AVR_SDS_TX_PORT, AVR_SDS_TX_PIN, PAL_MODE_OUTPUT_PUSHPULL);
  #ifdef AVR_SDS_USE_PCINT0
    /* Falling edge of INT0 triggers interrupt.*/
    EICRA |= (1 << ISC01);
    EICRA &= ~(1 << ISC00);
    /* Timer 2 CTC mode.*/
    TCCR2A |= 1 << WGM21;
    TCCR2A &= ~((1 << WGM22) | (1 << WGM20));
    /* Save the timer clock input.*/
    sds_rx_tccr2b_div = config->sc_tccr2b_div;
    /* Timer 2 Top.*/
    OCR2A = config->sc_ocr2a;
    /* Timer 2 output compare A interrupt.*/
    TIMSK2 |= 1 << OCIEA;
  #else
    #error "One of AVR_SDS_USE_PCINTx must be chosen"
  #endif
  // UBRR0L = config->sc_brr;
  // UBRR0H = config->sc_brr >> 8;
  // UCSR0A = 0;
  // UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);
  // switch (config->sc_bits_per_char) {
  // case USART_CHAR_SIZE_5:
  //   UCSR0C = 0;
  //   break;
  // case USART_CHAR_SIZE_6:
  //   UCSR0C = (1 << UCSZ00);
  //   break;
  // case USART_CHAR_SIZE_7:
  //   UCSR0C = (1 << UCSZ01);
  //   break;
  // case USART_CHAR_SIZE_9:
  //   UCSR0B |= (1 << UCSZ02);
  //   UCSR0C = (1 << UCSZ00) | (1 << UCSZ01);
  //   break;
  // case USART_CHAR_SIZE_8:
  // default:
  //   UCSR0C = (1 << UCSZ00) | (1 << UCSZ01);
  // }
}

/**
 * @brief   USART0 de-initialization.
 */
static void usartS_deinit(void) {

  // UCSR0A = 0;
  // UCSR0B = 0;
  // UCSR0C = 0;
}
#endif

#if 0
#if AVR_SERIAL_USE_USART1 || defined(__DOXYGEN__)
static void notify2(io_queue_t *qp) {

  (void)qp;
  UCSR1B |= (1 << UDRIE1);
}

/**
 * @brief   USART1 initialization.
 *
 * @param[in] config    the architecture-dependent serial driver configuration
 */
static void usart1_init(const SerialConfig *config) {

  UBRR1L = config->sc_brr;
  UBRR1H = config->sc_brr >> 8;
  UCSR1A = 0;
  UCSR1B = (1 << RXEN1) | (1 << TXEN1) | (1 << RXCIE1);
  switch (config->sc_bits_per_char) {
  case USART_CHAR_SIZE_5:
    UCSR1C = 0;
    break;
  case USART_CHAR_SIZE_6:
    UCSR1C = (1 << UCSZ10);
    break;
  case USART_CHAR_SIZE_7:
    UCSR1C = (1 << UCSZ11);
    break;
  case USART_CHAR_SIZE_9:
    UCSR1B |= (1 << UCSZ12);
    UCSR1C = (1 << UCSZ10) | (1 << UCSZ11);
    break;
  case USART_CHAR_SIZE_8:
  default:
    UCSR1C = (1 << UCSZ10) | (1 << UCSZ11);
  }
}

/**
 * @brief   USART1 de-initialization.
 */
static void usart1_deinit(void) {

  UCSR1A = 0;
  UCSR1B = 0;
  UCSR1C = 0;
}
#endif
#endif

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/
#if AVR_SERIAL_USE_USARTS || defined(__DOXYGEN__)
enum {
  IDLE,
  RECEIVE
} sds_rx_state_t;

static uint8_t sds_rx_tccr2b_div;

static volatile sds_rx_state_t sds_rx_state = IDLE;

inline void usartS_start_timer(void) {
  /* Reset counter.*/
  TCNT2 = 0;
  /* Start timer.*/
  TCCR2B &= ~AVR_SDS_RX_TCCR2B_CLK_MASK; /* Clear CLK section.*/
  TCCR2B |= sdx_rx_tccr2b_div; /* Set CLK setting.*/
}

inline void usartS_stop_timer(void) {
  TCCR2B &= ~AVR_SDS_RX_TCCR2B_CLK_MASK;
}

inline void usartS_reset_timer(void) {
  usartS_stop_timer();
  usartS_start_timer();
}

/**
 * @brief PCINT interrupt handler
 *
 * This handler changes state by sensing the START bit. Otherwise do nothing
 */
CH_FAST_IRQ_HANDLER(AVR_SDS_RX_VECT) {
  switch (sds_rx_state) {
    case IDLE:
      state = RECEIVE;
      usartS_start_timer();
    break;
    case RECEIVE:
      /* Do nothing.*/
    break;
  }
}

/**
 * @brief TIMER2 Comparator A interrupt
 *
 * Receives data if state is in RECEIVE
 */
CH_FAST_IRQ_HANDLER(TIMER2_COMPA_vect) {
  switch (sds_rx_state) {
    case IDLE:
      /* Do Nothing.*/
    break;
    case RECEIVE:
      /* TODO.*/
    break;
  }
}

#endif

#if 0
#if AVR_SERIAL_USE_USART0 || defined(__DOXYGEN__)
/**
 * @brief   USART0 RX interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(AVR_SD1_RX_VECT) {
  uint8_t sra;

  OSAL_IRQ_PROLOGUE();

  sra = UCSR0A;
  if (sra & ((1 << DOR0) | (1 << UPE0) | (1 << FE0)))
    set_error(sra, &SD1);
  osalSysLockFromISR();
  sdIncomingDataI(&SD1, UDR0);
  osalSysUnlockFromISR();

  OSAL_IRQ_EPILOGUE();
}

/**
 * @brief   USART0 TX interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(AVR_SD1_TX_VECT) {
  msg_t b;

  OSAL_IRQ_PROLOGUE();

  osalSysLockFromISR();
  b = sdRequestDataI(&SD1);
  osalSysUnlockFromISR();
  if (b < Q_OK)
    UCSR0B &= ~(1 << UDRIE0);
  else
    UDR0 = b;

  OSAL_IRQ_EPILOGUE();
}
#endif /* AVR_SERIAL_USE_USART0 */

#if AVR_SERIAL_USE_USART1 || defined(__DOXYGEN__)
/**
 * @brief   USART1 RX interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(AVR_SD2_RX_VECT) {
  uint8_t sra;

  OSAL_IRQ_PROLOGUE();

  sra = UCSR1A;
  if (sra & ((1 << DOR1) | (1 << UPE1) | (1 << FE1)))
    set_error(sra, &SD2);
  osalSysLockFromISR();
  sdIncomingDataI(&SD2, UDR1);
  osalSysUnlockFromISR();

  OSAL_IRQ_EPILOGUE();
}

/**
 * @brief   USART1 TX interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(AVR_SD2_TX_VECT) {
  msg_t b;

  OSAL_IRQ_PROLOGUE();

  osalSysLockFromISR();
  b = sdRequestDataI(&SD2);
  osalSysUnlockFromISR();
  if (b < Q_OK)
    UCSR1B &= ~(1 << UDRIE1);
  else
    UDR1 = b;

  OSAL_IRQ_EPILOGUE();
}
#endif /* AVR_SERIAL_USE_USART1 */
#endif
/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level serial driver initialization.
 *
 * @notapi
 */
void sd_lld_init(void) {

#if AVR_SERIAL_USE_USARTS
  sdObjectInit(&SDS, NULL, notifyS);
#endif
}

/**
 * @brief   Low level serial driver configuration and (re)start.
 *
 * @param[in] sdp       pointer to a @p SerialDriver object
 * @param[in] config    the architecture-dependent serial driver configuration.
 *                      If this parameter is set to @p NULL then a default
 *                      configuration is used.
 *
 * @notapi
 */
void sd_lld_start(SerialDriver *sdp, const SerialConfig *config) {

  if (config == NULL)
    config = &default_config;

#if AVR_SERIAL_USE_USART0
  if (&SD1 == sdp) {
    usart0_init(config);
    return;
  }
#endif
#if AVR_SERIAL_USE_USART1
  if (&SD2 == sdp) {
    usart1_init(config);
    return;
  }
#endif
}

/**
 * @brief   Low level serial driver stop.
 * @details De-initializes the USART, stops the associated clock, resets the
 *          interrupt vector.
 *
 * @param[in] sdp       pointer to a @p SerialDriver object
 *
 * @notapi
 */
void sd_lld_stop(SerialDriver *sdp) {

#if AVR_SERIAL_USE_USART0
  if (&SD1 == sdp)
    usart0_deinit();
#endif
#if AVR_SERIAL_USE_USART1
  if (&SD2 == sdp)
    usart1_deinit();
#endif
}

#endif /* HAL_USE_SERIAL */

/** @} */
