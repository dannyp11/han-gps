#include <ctype.h>
#include <errno.h>
#include <stdlib.h>

#include "parser.h"

#include "debug.h"

PARSE_FUNC(uint16) {
  char *end;
  uint8_t result = strtoul((char *)buf, &end, 10);
  if (errno == ERANGE) {
    return PARSE_FAILED;
  }
  return PARSE_SUCCESS;
}

/**
 * @brief Actually steps a parser, which is configured by three tables.
 */
void stepParser(msg_t c,
                size_t parserSize,
                const parser_t *parserTable,
                void (*cleanup)(void)) {
  /* Which parser is being used.*/
  static uint8_t parserState = 0;
  /* Counter for individual parser.*/
  static uint8_t i = 0;
  /* Stores partial match result.*/
  static msg_t buf[16];
  /* The current parser.*/
  parser_t p = parserTable[parserState];
  /* The current matcher.*/
  match_func_t match = p.matcher;
  /* The current parser.*/
  parse_func_t parse = p.parser;
  /* The current writeback.*/
  writeback_t wb = p.writeback;
  /* Parse result.*/
  match_result_t match_result = match(c, i);
  debug("|%d,%d:%c,%d.", parserState, i, c, match_result);
  /* Save the byte.*/
  buf[i] = c;
  switch (match_result) {
  case MATCH_PARTIAL:
    /* Increment counter current matcher.*/
    ++i;
    break;
  case MATCH_SUCCESS:
    /* Terminate string if necessary.*/
    if (++i < 16)
      buf[i] = '\0';
    /* Perform parsing.*/
    if (parse != NULL) {
      if (!parse(buf, i, wb))
        goto failure;
    }
    /* Reset counter for next matcher.*/
    i = 0;
    /* Move to next matcher. Restart if appropriate.*/
    if (++parserState >= parserSize)
      parserState = 0;
    break;
  default:
    goto failure;
  }
  return;
/* If anything fails, reset.*/
failure:
  /* Reset counter for next matcher.*/
  i = 0;
  /* Start over.*/
  parserState = 0;
  cleanup();
}

MATCH_CHAR_FUNC(Comma, ',');
MATCH_CHAR_FUNC(Dollar, '$');
MATCH_CHAR_FUNC(CR, '\r');
MATCH_CHAR_FUNC(LF, '\n');
MATCH_FUNC(UpperCase) {
  return (c >= 'A' && c <= 'Z');
}

MATCH_FUNC(Digit) {
  return isdigit(c);
}

MATCH_FUNC(Decimal) {
  return match_Digit(c, i) || c == '.';
}