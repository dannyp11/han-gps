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

#ifndef _BOARD_H_
#define _BOARD_H_

/*
 * Setup for the Arduino Uno or  board.
 */

/*
 * Board identifier.
 */
#define BOARD_HAN
#define BOARD_NAME "Hillary And Norgay"

#if defined(BOARD_USE_UNO)
#include "board_uno.h"
#elif defined(BOARD_USE_CUSTOM)
#include "board_custom.h"
#else
#error "Must specify -DBOARD_USE_UNO or -DBOARD_USE_CUSTOM"
#endif

#if !defined(_FROM_ASM_)
#ifdef __cplusplus
extern "C" {
#endif
  void boardInit(void);
#ifdef __cplusplus
}
#endif
#endif /* _FROM_ASM_ */

#endif /* _BOARD_H_ */
