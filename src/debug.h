#ifndef __DEBUG_H__
#define __DEBUG_H__

#include "hal.h"
#include "chprintf.h"

#ifdef USE_DEBUG
#define debug(...) \
chprintf((BaseSequentialStream *)&SD1, __VA_ARGS__)
#define debugS(...) \
chprintf((BaseSequentialStream *)&SDS, __VA_ARGS__)
#else
#define debug(...)
#define debugS(...)
#endif

#endif