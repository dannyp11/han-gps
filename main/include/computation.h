#pragma once

#include "ch.h"
#include "hal.h"

#define COMP_WA_SIZE 128

extern THD_WORKING_AREA(waTdComp, COMP_WA_SIZE);
extern THD_FUNCTION(tdComp, arg);