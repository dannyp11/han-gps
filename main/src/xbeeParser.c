#include "xbeeParser.h"
#include "gpsParser.h"
#include "parser.h"
#include <stdlib.h>

static mailbox_t *mailbox = NULL;
static memory_pool_t *pool = NULL;
static int8_t peerIDX = INVALID_XBEE_DATA;
static deg_min_t longitudeX = {INVALID_XBEE_DATA, INVALID_XBEE_DATA};
static deg_min_t latitudeX = {INVALID_XBEE_DATA, INVALID_XBEE_DATA};
static int8_t msgTypeX = INVALID_XBEE_DATA;

void xbeeSetCallback(mailbox_t *mb, memory_pool_t *mp) {
  info_xbee("XBee Set Callback\r\n");
  mailbox = mb;
  pool = mp;
}

int8_t xbeeGetID() {
  return peerIDX;
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
  if (i < 2 && isdigit(c)) {
    return MATCH_PARTIAL;
  } else if (i == 2 && c == ',') {
    return MATCH_SUCCESS;
  } else {
    return MATCH_FAILED;
  }
}

PARSE_FUNC(XbeeFinalize) {
  msg_t r;
  peer_message_t *p;
  info_xbee("Xbee Finalize\r\n");
  // info_xbee("Longitude Deg: %D, Longitude Min: %D", longitudeX.degree, longitudeX.minute);
  /* If there is a callback, then fire an event.*/
  if (mailbox != NULL && pool != NULL) {
    p = chPoolAlloc(pool);
    /* If allocation is successful.*/
    if (p != NULL) {
      info_xbee("XBee Finalize size=%d p=%d\r\n", sizeof(p), p);
      p->peerID = peerIDX;
      p->longitude = longitudeX;
      p->latitude = latitudeX;
      p->msgID = msgTypeX;
      r = chMBPost(mailbox, (msg_t)p, TIME_IMMEDIATE);
    }
  }
  if (p == NULL) {
    info_xbee("Xbee Finalize Failed\r\n");
    return PARSE_FAILED;
  } else if (r != MSG_OK) {
    chPoolFree(pool, p);
    info_xbee("Xbee Finalize Failed\r\n");
    return PARSE_FAILED;
  } else {
    info_xbee("Xbee Finalize Succeeded\r\n");
    return PARSE_SUCCESS;
  }
}

parser_t xbeeParser(parserstate_t parserState) {
  switch (parserState) {
  case 0:
    return new_parser(match_Asteroid, NULL, NULL);
  case 1:
    return new_parser(match_MsgID, parse_DigitWithComma, (writeback_t)&peerIDX);
  case 2:
    return new_parser(match_DegMin, parse_DegMin, (writeback_t)&longitudeX);
  case 3:
    return new_parser(match_DegMin, parse_DegMin, (writeback_t)&latitudeX);
  case 4:
    return new_parser(match_Digit, parse_Digit, (writeback_t)&msgTypeX);
  case 5:
    return new_parser(match_Asteroid, parse_XbeeFinalize, NULL);
  default:
    return new_parser(NULL, NULL, NULL);
  }
}

void xbeeParserCleanup(void) {
  longitudeX.degree = INVALID_XBEE_DATA;
  longitudeX.minute = INVALID_XBEE_DATA;
  latitudeX.degree = INVALID_XBEE_DATA;
  latitudeX.minute = INVALID_XBEE_DATA;
  peerIDX = INVALID_XBEE_DATA;
  msgTypeX = INVALID_XBEE_DATA;
}

static char xbeeBuf[16];
static parserstate_t xbeeParserState = 0;
static parserstate_t xbeeCount = 0;

void xbeeStepParser(msg_t c) {
  stepParser(
      c, xbeeParser, xbeeParserCleanup, xbeeBuf, 16, &xbeeParserState, &xbeeCount);
}