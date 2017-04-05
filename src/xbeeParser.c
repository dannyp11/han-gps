#include "xbeeParser.h"
#include "parser.h"
#include "gpsParser.h"
#include <stdlib.h>

static thread_t *callbackThdX = NULL;
static int8_t msgIDX = INVALID_XBEE_DATA;
static deg_min_t longitudeX = {INVALID_XBEE_DATA, INVALID_XBEE_DATA};
static deg_min_t latitudeX = {INVALID_XBEE_DATA, INVALID_XBEE_DATA};
static int8_t msgTypeX = INVALID_XBEE_DATA;

void xbeeSetCallbackThread(thread_t *td) {
    callbackThdX = td;
}

int8_t xbeeGetID() {
    return msgIDX;
}

deg_min_t xbeeGetLongitude() {
    return longitudeX;
}

deg_min_t xbeeGetLatitude() {
    return latitudeX;
}

int8_t xbeeGetMessage() {
    return msgTypeX;
}

MATCH_FUNC(MsgID) {
    if (i <= 2 && isdigit(c)) {
        return MATCH_PARTIAL;
    }
    else if (i == 3 && c == ',') {
        return MATCH_SUCCESS;
    }
    else {
        return MATCH_FAILED;
    }
}

PARSE_FUNC(XbeeFinalize) {
    /* If there is a callback, then fire an event.*/
    if (callbackThdX != NULL) {

    }
    return PARSE_SUCCESS;
}

parser_t xbeeParser(parserstate_t parserState) {
    switch (parserState) {
        case 0: return new_parser(match_Asteroid, NULL, NULL);
        case 1: return new_parser(match_MsgID, parse_DigitWithComma, (writeback_t)&msgIDX);
        case 2: return new_parser(match_DegMin, parse_DegMin, (writeback_t)&longitudeX);
        case 3: return new_parser(match_DegMin, parse_DegMin, (writeback_t)&latitudeX);
        case 4: return new_parser(match_Digit, parse_Digit, (writeback_t) &msgTypeX);
        case 5: return new_parser(match_Asteroid, parse_XbeeFinalize, NULL);
        default: return new_parser(NULL, NULL, NULL);
    }
}

void xbeeParserCleanup(void) {
    longitudeX.degree = INVALID_XBEE_DATA;
    longitudeX.minute = INVALID_XBEE_DATA;
    latitudeX.degree = INVALID_XBEE_DATA;
    latitudeX.minute = INVALID_XBEE_DATA;
    msgIDX = INVALID_XBEE_DATA;
    msgTypeX = INVALID_XBEE_DATA;
}

static char xbeeBuf[16];
static parserstate_t xbeeParserState = 0;
static parserstate_t xbeeCount = 0;

void xbeeStepParser(msg_t c) {
    stepParser(
        c, xbeeParser, xbeeParserCleanup, xbeeBuf, 16, &xbeeParserState, &xbeeCount
    );
}