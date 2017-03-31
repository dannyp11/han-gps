#ifndef __DEBUG_H__
#define __DEBUG_H__

#include <stdio.h>

typedef signed char msg_t;

#define debug(...) \
printf(__VA_ARGS__)


#endif