#include "softserialcfg.h"

#if defined(BOARD_USE_CUSTOM)
/**
 * @brief B4800 on 7372800Hz Clock
 */
const SerialConfig softserial_config = {
  UBRR(4800), /* No use, just a placeholder.*/
  USART_CHAR_SIZE_8,
  96,
  (1 << CS21) /* Divide 8.*/
};
#elif defined(BOARD_USE_UNO)
/**
 * @brief B4800(approx) on 16000000Hz Clock
 */
const SerialConfig softserial_config = {
  UBRR(4800), /* No use, just a placeholder.*/
  USART_CHAR_SIZE_8,
  208, 
  (1 << CS21) /* Divide 8.*/
};
#else
#endif