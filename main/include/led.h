#pragma once 

#include "ch.h"
#include "LED_ll.h"

#define LED_WA_SIZE 32
extern THD_WORKING_AREA(waTdLED, LED_WA_SIZE);
extern THD_FUNCTION(tdLED, arg);