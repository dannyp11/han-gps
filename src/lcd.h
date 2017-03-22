#pragma once 

#include "ch.h"
#include "hal.h"

#define LCD_WA_SIZE 128

extern THD_WORKING_AREA(waTdLCD, LCD_WA_SIZE);
extern THD_FUNCTION(tdLCD, arg);