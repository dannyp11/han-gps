#pragma once

#include "ch.h"
#include "hal.h"

#define COMP_WA_SIZE 128


#define ALERT_NONE -1

#define MAX_PEERS 8

typedef struct {
    int16_t bearing;
    int16_t distance;
} alert_message_t;

float getMyLongitude(void);
float getMyLatitude(void);

extern THD_WORKING_AREA(waTdComp, COMP_WA_SIZE);
extern THD_FUNCTION(tdComp, arg);
