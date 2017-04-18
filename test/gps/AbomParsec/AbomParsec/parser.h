#ifndef __PARSER_H__
#define __PARSER_H__

//#include "ch.h"

#include <ctype.h>
#include <inttypes.h>

#include "debug.h"

/*
 * Matchers
 */

/**
 * @brief 1 if matched, 0 if fail, -1 if partial match 
 */
typedef int8_t match_result_t;
#define MATCH_SUCCESS 1
#define MATCH_FAILED 0
#define MATCH_PARTIAL -1

/**
 * @brief Type of a matching function
 */
typedef match_result_t (*match_func_t)(char, uint8_t);

/**
 * @brief A generic, multi-character match function
 */
#define MATCH_FUNC(name) match_result_t match_##name(char c, uint8_t i)

#define MATCH_CHAR_FUNC(name, c0) \
  \
MATCH_FUNC(name) {                \
    return c0 == c;               \
  \
}

#define MATCH_ANY(name, count) \
  \
MATCH_FUNC(name) {             \
    if (++i == count)          \
      return MATCH_SUCCESS;    \
    else if (i < count)        \
      return MATCH_PARTIAL;    \
    else                       \
      return MATCH_FAILED;     \
  \
}

#define MATCH_UNTIL(name, c0, maxcnt)                       \
  \
MATCH_FUNC(name) {                                  \
    return (i < maxcnt && c == c0) ? MATCH_SUCCESS : MATCH_PARTIAL; \
  \
}

extern MATCH_FUNC(Comma);
extern MATCH_FUNC(Dollar);
extern MATCH_FUNC(CR);
extern MATCH_FUNC(LF);

extern MATCH_FUNC(UpperCase);
extern MATCH_FUNC(Digit);
extern MATCH_FUNC(Decimal);

/*
 * Parsers
 */

/**
 * @brief 1 if matched, 0 if fail, -1 if partial match 
 */
typedef int8_t parse_result_t;
#define PARSE_SUCCESS 1
#define PARSE_FAILED 0
#define PARSE_PARTIAL -1

/**
 * @brief Writeback pointer
 */
typedef void *writeback_t;

typedef uint8_t parserstate_t;

/**
 * @brief Type of a parser function. The void pointer is the item to be written.
 */
typedef parse_result_t (*parse_func_t)(char *, parserstate_t, writeback_t);

#define PARSE_FUNC(name) parse_result_t parse_##name(char *buf, uint8_t length, void *write_back)

PARSE_FUNC(uint16);

typedef struct {
  match_func_t matcher;
  parse_func_t parser;
  writeback_t writeback;
} parser_t;

static inline parser_t new_parser(match_func_t m, parse_func_t p, writeback_t w) {
  parser_t par = {m, p, w};
  return par;
}

typedef parser_t (* parser_functable_t)(parserstate_t);

void stepParser(msg_t c,
                parser_functable_t parserTable,
                void (*cleanup)(void),
                char *buf,
                size_t bufsize,
                parserstate_t *parserState,
                parserstate_t *i);

#endif