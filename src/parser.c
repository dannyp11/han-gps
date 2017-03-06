#include <stdlib.h>
#include <errno.h>

#include "parser.h"

PARSE_FUNC(uint16) {
    char *end;
    uint8_t result = strtoul(buf, &end, 10);
    if (errno == ERANGE){
        return PARSE_FAILED;
    }
    return PARSE_SUCCESS;
}