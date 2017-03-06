#ifndef __MONITOR_H__
#define __MONITOR_H__

#include "ch.h"

#define MONITOR_WA_SIZE 64

extern THD_WORKING_AREA(waTdMon, MONITOR_WA_SIZE);
extern THD_FUNCTION(tdMon, arg);

#endif 