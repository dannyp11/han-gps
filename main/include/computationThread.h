#pragma once

#include "ch.h"
#include "hal.h"

#if DEBUG_COMPUTATION
#define COMP_WA_SIZE 128
#else 
#define COMP_WA_SIZE 128
#endif

#define ALERT_NONE -1

#define MAX_PEERS 8

typedef struct {
    int16_t bearing;
    int16_t distance;
} alert_message_t;

extern THD_WORKING_AREA(waTdComp, COMP_WA_SIZE);
extern THD_FUNCTION(tdComp, arg);