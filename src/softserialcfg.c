#include "softserialcfg.h"

#if defined(BOARD_USE_CUSTOM)
/**
 * @brief B9600 on 7372800Hz Clock
 */
const SerialConfig softserial_config = {
  UBRR(9600), /* No use, just a placeholder.*/
  USART_CHAR_SIZE_8,
  96,
  (1 << CS21) /* Divide 8.*/
};
#elif defined(BOARD_USE_UNO)
/**
 * @brief B9600(approx) on 16000000Hz Clock
 */
const SerialConfig softserial_config = {
  UBRR(9600), /* No use, just a placeholder.*/
  USART_CHAR_SIZE_8,
  208,
  (1 << CS21) /* Divide 8.*/
};
#else
#endif
