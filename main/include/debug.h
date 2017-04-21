#ifndef __DEBUG_H__
#define __DEBUG_H__

#include "hal.h"
#include "chprintf.h"

#if defined(USE_DEBUG) && DEBUG_INFO
#define debug(...) \
chprintf((BaseSequentialStream *)&SD1, __VA_ARGS__)
#define info(...) \
debug("(II) " __VA_ARGS__)
#define debugS(...) \
chprintf((BaseSequentialStream *)&SDS, "[SDS] " __VA_ARGS__)
#define infoS(...) \
debugS("(II) " __VA_ARGS__)

#else
#define debug(...)
#define info(...)
#define debugS(...)
#define infoS(...)
#endif

#if DEBUG_XBEE
#define debug_xbee(...) debug("[XBee] " __VA_ARGS__)
#define info_xbee(...) info("[XBee] " __VA_ARGS__)
#else 
#define debug_xbee(...)
#define info_xbee(...)
#endif

#if DEBUG_GPS
#define debug_gps(...) debug("[GPS] " __VA_ARGS__)
#define info_gps(...) info("[GPS] " __VA_ARGS__)
#else 
#define debug_gps(...)
#define info_gps(...)
#endif

#if DEBUG_PARSER
#define debug_parser(...) debug("[Parser] " __VA_ARGS__)
#define info_parser(...) info("[Parser] " __VA_ARGS__)
#else 
#define debug_parser(...)
#define info_parser(...)
#endif 

#if DEBUG_COMPUTATION
#define debug_computation(...) debug("[Comp] " __VA_ARGS__)
#define info_computation(...) info("[Comp] " __VA_ARGS__)
#else 
#define debug_computation(...)
#define info_computation(...)
#endif 

#endif