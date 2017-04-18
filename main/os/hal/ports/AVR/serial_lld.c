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
 * @file    AVR/serial_lld.c
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

/**
 * @brief   Software serial driver identifier
 */
#if AVR_SERIAL_USE_USARTS || defined(__DOXYGEN__)
SerialDriver SDS;
#if !AVR_GPT_USE_TIM2 && !((OSAL_ST_MODE == OSAL_ST_MODE_PERIODIC) && AVR_GPT_USE_TIM1) && !((OSAL_ST_MODE == OSAL_ST_MODE_FREERUNNING) && AVR_GPT_USE_TIM1)
#error "Software serial requires AVR_GPT_USE_TIM1, AVR_GPT_USE_TIM2, or AVR_GPT_USE_TIM3, given they don't conflict with system timer."
#endif

/* Uses INT0*/
#if AVR_SDS_USE_INT0
#define AVR_SDS_RX_PORT IOPORT4
#define AVR_SDS_RX_PIN 2
#define AVR_SDS_RX_VECT INT0_vect
#endif

#define AVR_SDS_RX_TCCRxB_CLK_MASK 0b00000111
#if AVR_SDS_USE_TIMER == 0
#define TCNTx TCNT0
#define TCCRxA TCCR0A
#define TCCRxB TCCR0B
#define OCRxA OCR0A
#define OCRxB OCR0B
#define TIMSKx TIMSK0
#define OCIExA OCIE0A
#define OCIExB OCIE0B
#define USARTS_TIM_vect TIMER0_COMPA_vect
#elif AVR_SDS_USE_TIMER == 1
#define TCNTx TCNT1
#define TCCRxA TCCR1A
#define TCCRxB TCCR1B
#define OCRxA OCR1A
#define OCRxB OCR1B
#define TIMSKx TIMSK1
#define OCIExA OCIE1A
#define OCIExB OCIE1B
#define USARTS_TIM_vect TIMER1_COMPA_vect
#elif AVR_SDS_USE_TIMER == 2
#define TCNTx TCNT2
#define TCCRxA TCCR2A
#define TCCRxB TCCR2B
#define OCRxA OCR2A
#define OCRxB OCR2B
#define TIMSKx TIMSK2
#define OCIExA OCIE2A
#define OCIExB OCIE2B
#define USARTS_TIM_vect TIMER2_COMPA_vect
#else
#error "You must specify AVR_SDS_USE_TIMER as 0, 1, or 2."
#endif

/* By default, uses PB1 as TX.*/
#if !defined(AVR_SDS_TX_PORT)
#define AVR_SDS_TX_PORT IOPORT4
#endif
#if !defined(AVR_SDS_TX_PIN)
#define AVR_SDS_TX_PIN 3
#endif
#endif

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/**
 * @brief SDS state machine
 */
typedef enum {
  SDS_RX_IDLE,
  SDS_RX_WAIT,
  SDS_RX_SAMPLE
} sds_rx_state_t;

typedef enum {
  SDS_TX_IDLE,
  SDS_TX_TRANSMIT,
  SDS_TX_WAIT
} sds_tx_state_t;

/**
 * @brief   Driver default configuration.
 */
static const SerialConfig default_config = {
    UBRR(SERIAL_DEFAULT_BITRATE),
    USART_CHAR_SIZE_8,
    96,
    (1 << 1)};

/**
 * @brief SDS Timer clock control value
 */
static uint8_t sds_rx_TCCRxB_div;

/**
 * @brief SDS UART bits per character
 */
static uint8_t sds_bits_per_char;

static volatile sds_rx_state_t sds_rx_state = SDS_RX_IDLE;

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

static void set_error(uint8_t sra, SerialDriver *sdp) {
  eventflags_t sts = 0;
  uint8_t dor = 0;
  uint8_t upe = 0;
  uint8_t fe = 0;

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

#if AVR_SERIAL_USE_USART0 || defined(__DOXYGEN__)
static void notify1(io_queue_t *qp) {

  (void)qp;
  UCSR0B |= (1 << UDRIE0);
}

/**
 * @brief   USART0 initialization.
 *
 * @param[in] config    the architecture-dependent serial driver configuration
 */
static void usart0_init(const SerialConfig *config) {

  UBRR0L = config->sc_brr;
  UBRR0H = config->sc_brr >> 8;
  UCSR0A = 0;
  UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);
  switch (config->sc_bits_per_char) {
  case USART_CHAR_SIZE_5:
    UCSR0C = 0;
    break;
  case USART_CHAR_SIZE_6:
    UCSR0C = (1 << UCSZ00);
    break;
  case USART_CHAR_SIZE_7:
    UCSR0C = (1 << UCSZ01);
    break;
  case USART_CHAR_SIZE_9:
    UCSR0B |= (1 << UCSZ02);
    UCSR0C = (1 << UCSZ00) | (1 << UCSZ01);
    break;
  case USART_CHAR_SIZE_8:
  default:
    UCSR0C = (1 << UCSZ00) | (1 << UCSZ01);
  }
}

/**
 * @brief   USART0 de-initialization.
 */
static void usart0_deinit(void) {

  UCSR0A = 0;
  UCSR0B = 0;
  UCSR0C = 0;
}
#endif

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

#if AVR_SERIAL_USE_USARTS || defined(__DOXYGEN__)

void usartS_start_timer(void) {
  /* Reset counter.*/
  TCNTx = 0;
  /* Start timer.*/
  TCCRxB &= ~AVR_SDS_RX_TCCRxB_CLK_MASK; /* Clear CLK section.*/
  TCCRxB |= sds_rx_TCCRxB_div;           /* Set CLK setting.*/
}

void usartS_stop_timer(void) {
  TCCRxB &= ~AVR_SDS_RX_TCCRxB_CLK_MASK;
}

void usartS_reset_timer(void) {
  usartS_stop_timer();
  usartS_start_timer();
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
#if AVR_SDS_USE_INT0
  /* Falling edge of INT0 triggers interrupt.*/
  EICRA |= (1 << ISC01);
  EICRA &= ~(1 << ISC00);
  EIMSK |= 1 << 0;
#endif
/* Timer CTC mode.*/
#if AVR_SDS_USE_TIMER == 0
#elif AVR_SDS_USE_TIMER == 1
  TCCRxB |= 1 << WGM12;
  TCCRxB &= ~(1 << WGM13);
  TCCRxA &= ~((1 << WGM11) | (1 << WGM10));
#elif AVR_SDS_USE_TIMER == 2
  TCCRxA |= 1 << WGM21;
  TCCRxA &= ~((1 << WGM22) | (1 << WGM20));
#endif
  /* Save the timer clock input.*/
  sds_rx_TCCRxB_div = config->sc_tccrxb_div;
  /* Default to be 8 bit.*/
  switch (config->sc_bits_per_char) {
  default:
    sds_bits_per_char = 8;
  }

  /* Timer 2 Top.*/
  OCRxA = config->sc_ocrxa;
  /* Timer 2 output compare A interrupt.*/
  TIMSKx |= 1 << OCIExA;
  usartS_start_timer();
}

/**
* @brief   USART0 de-initialization.
*/
static void usartS_deinit(void) {
  usartS_stop_timer();
  TIMSKx &= ~(1 << OCIExA);
  EIMSK &= ~(1 << 0);
}

#endif

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

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

#if AVR_SERIAL_USE_USARTS || defined(__DOXYGEN__)

/**
 * @brief PCINT interrupt handler
 *
 * @details This handler changes state by sensing the START bit. Otherwise do nothing
 *
 * @isr
 */
OSAL_IRQ_HANDLER(AVR_SDS_RX_VECT) {
  OSAL_IRQ_PROLOGUE();
  chSysDisable();
  switch (sds_rx_state) {
  case SDS_RX_IDLE:
    sds_rx_state = SDS_RX_WAIT;
    break;
  default:
    break;
  }
  chSysEnable();
  OSAL_IRQ_EPILOGUE();
}

/**
 * @brief TIMER2 Comparator A interrupt
 *
 * @details VERY IMPORTANT: Timer is triggered twice per data bit, so 4800 BAUD 
 * serial requires 9600Hz clock.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(USARTS_TIM_vect) {
  /* Data byte.*/

  OSAL_IRQ_PROLOGUE();
  {
    static int8_t rx_i;
    static uint8_t rx_byte;
    /* RX state machine.*/
    switch (sds_rx_state) {
    case SDS_RX_IDLE:
      rx_i = -1;
      rx_byte = 0;
      /* Do Nothing.*/
      break;
    case SDS_RX_WAIT: /* Waits a clock before sampling*/
      // byte = 0;
      sds_rx_state = SDS_RX_SAMPLE;
      break;
    case SDS_RX_SAMPLE:
      if (rx_i < 0) {
        /* Do nothing, START.*/
      } else if (rx_i < sds_bits_per_char) {
        rx_byte |= palReadPad(AVR_SDS_RX_PORT, AVR_SDS_RX_PIN) << rx_i;
      } else {
        /* If last bit is STOP, then assume info is correct. Otherwise, treat as garbage*/
        if (palReadPad(AVR_SDS_RX_PORT, AVR_SDS_RX_PIN)) {
          osalSysLockFromISR();
          sdIncomingDataI(&SDS, rx_byte);
          osalSysUnlockFromISR();
        }
        rx_byte = 0;
      }
      if (rx_i < sds_bits_per_char) {
        sds_rx_state = SDS_RX_WAIT;
      } else {
        sds_rx_state = SDS_RX_IDLE;
      }
      ++rx_i;
      break;
    }
  }
  /* TX state machine.*/
  {
    static sds_tx_state_t tx_state = SDS_TX_IDLE;
    static int8_t tx_byte;
    static int8_t tx_i;
    switch (tx_state) {
    case SDS_TX_IDLE:
      tx_i = -1;
      osalSysLockFromISR();
      tx_byte = sdRequestDataI(&SDS);
      osalSysUnlockFromISR();
      if (tx_byte >= Q_OK) {
        tx_state = SDS_TX_TRANSMIT;
      }
      break;
    case SDS_TX_TRANSMIT: {
      uint8_t bit;
      /* START.*/
      if (tx_i == -1) {
        bit = 0;
      }
      /* STOP.*/
      else if (tx_i == sds_bits_per_char) {
        bit = 1;
      }
      /* Data.*/
      else {
        bit = (tx_byte & (1 << tx_i)) != 0;
      }
      palWritePad(AVR_SDS_TX_PORT, AVR_SDS_TX_PIN, bit);
      tx_state = SDS_TX_WAIT;
      break;
    }
    case SDS_TX_WAIT:
      if (tx_i == sds_bits_per_char) {
        tx_state = SDS_TX_IDLE;
      } else {
        tx_state = SDS_TX_TRANSMIT;
      }
      ++tx_i;
      break;
    }
  }
  OSAL_IRQ_EPILOGUE();
}

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

#if AVR_SERIAL_USE_USART0
  sdObjectInit(&SD1, NULL, notify1);
#endif
#if AVR_SERIAL_USE_USART1
  sdObjectInit(&SD2, NULL, notify2);
#endif
#if AVR_SERIAL_USE_USARTS
  sdObjectInit(&SDS, NULL, NULL);
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
#if AVR_SERIAL_USE_USARTS
  if (&SDS == sdp) {
    usartS_init(config);
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
#if AVR_SERIAL_USE_USARTS
  if (&SDS == sdp) {
    usartS_deinit();
  }
#endif
}

#endif /* HAL_USE_SERIAL */

/** @} */
