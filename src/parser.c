#include <ctype.h>
#include <errno.h>
#include <stdlib.h>

#include <string.h>

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
                parser_functable_t parserTable,
                void (*cleanup)(void),
                char *buf,
                size_t bufsize,
                parserstate_t *parserState,
                parserstate_t *i) {
  uint8_t j = 0;
  /* The current parser.*/
  parser_t p = parserTable(*parserState);
  /* The current matcher.*/
  match_func_t match = p.matcher;

  /* The current parser.*/
  parse_func_t parse = p.parser;
  /* The current writeback.*/
  writeback_t wb = p.writeback;
  /* Parse result.*/
  match_result_t match_result = match(c, *i);

  if (*i >= bufsize - 1)
    goto failure;
  /* Save the byte.*/
  buf[*i] = c;
  buf[*i + 1] = '\0';
  // debug("\"");
  // for (j = 0; j < bufsize; ++j) {
  //   debug("%c", buf[j]);
  // }
  // debug(",");
  // for (j = 0; j < bufsize; ++j) {
  //   debug("\\%02x", buf[j]);
  // }
  // debug("\"\r\n");
  //debug("|%d,%d:%c,%d,\"%s\".\r\n", *parserState, *i, c, match_result, (char*)buf);

  //debug("|%d,%d:%c,%d.", parserState, i, c, match_result);
  switch (match_result) {
  case MATCH_PARTIAL:
    /* Increment counter current matcher.*/
    ++*i;
    break;
  case MATCH_SUCCESS:
    /* Terminate string if necessary.*/
    if (++*i < bufsize)
      buf[*i] = '\0';
    /* Perform parsing.*/
    if (parse != NULL) {
      if (!parse(buf, *i, wb))
        goto failure;
    }
    /* Reset counter for next matcher.*/
    *i = 0;
    /* Reset state if entire message is parsed.*/
    if (parserTable(++*parserState).matcher == NULL) {
      *parserState = 0;
      return;
    }
    break;
  default:
    goto failure;
  }
  return;
/* If anything fails, reset.*/
failure:
  /* Reset counter for next matcher.*/
  *i = 0;
  /* Start over.*/
  *parserState = 0;
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