#ifndef __PARSER_H__
#define __PARSER_H__

#include <ctype.h>
#include "ch.h"

/*
 * Matchers
 */

/**
 * @brief 1 if matched, 0 if fail, -1 if partial match 
 */
typedef int8_t match_result_t;
#define MATCH_SUCCESS 1
#define MATCH_FAILED  0
#define MATCH_PARTIAL -1

/**
 * @brief Type of a matching function
 */
typedef match_result_t (* match_func_t)(msg_t, uint8_t);

/**
 * @brief A generic, multi-character match function
 */
#define MATCH_FUNC(name) match_result_t match_##name(msg_t c, uint8_t i)

#define MATCH_CHAR_FUNC(name, c0) \
MATCH_FUNC(name) { \
  return c0 == 0; \
} 

#define MATCH_ANY(name, count) \
MATCH_FUNC(name) { \
    return MATCH_SUCCESS; \
}

extern MATCH_FUNC(Comma);
extern MATCH_FUNC(Dollar);
extern MATCH_FUNC(CR);
extern MATCH_FUNC(LF);


extern MATCH_FUNC(UpperCase);

inline MATCH_FUNC(Digit) {
    return isdigit(c);
}

inline MATCH_FUNC(Decimal) {
    return match_Digit(c, i) || c == '.';
}

/*
 * Parsers
 */

/**
 * @brief 1 if matched, 0 if fail, -1 if partial match 
 */
typedef int8_t parse_result_t;
#define PARSE_SUCCESS 1
#define PARSE_FAILED  0
#define PARSE_PARTIAL -1

/**
 * @brief Writeback pointer
 */
 typedef void * writeback_t;

/**
 * @brief Type of a parser function. The void pointer is the item to be written.
 */
typedef parse_result_t (* parse_func_t)(msg_t *, uint8_t, writeback_t);

#define PARSE_FUNC(name) parse_result_t parse_##name(msg_t *buf, uint8_t length, void *write_back)

PARSE_FUNC(uint16);

void stepParser(msg_t c,
                size_t parserSize,
                const match_func_t *matchFuncTable,
                const parse_func_t *parseFuncTable,
                const writeback_t *writebackTable,
                void (*cleanup)(void));

#endif