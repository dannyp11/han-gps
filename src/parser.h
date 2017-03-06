#ifndef __PARSER_H__
#define __PARSER_H__

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
#define PARSER_MATCH_FUNC(name) match_result_t match_##name(msg_t c, uint8_t i)
#define MATCH_CHAR_FUNC(name, c0) \
inline PARSER_MATCH_FUNC(name) { \
  return c0 == 0; \
} 

MATCH_CHAR_FUNC(G, 'G');
MATCH_CHAR_FUNC(A, 'A');
MATCH_CHAR_FUNC(Comma, ',');
MATCH_CHAR_FUNC(Dollar, '$');

inline PARSER_MATCH_FUNC(UpperCase) {
  return (c >= 'A' && c <= 'Z');
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
 * @brief Type of a parser function. The void pointer is the item to be written.
 */
typedef parse_result_t (* parse_func_t)(msg_t *, uint8_t, void *);


#define PARSE_FUNC(name) parse_result_t parse_##name(msg_t *buf, uint8_t length, void *write_back)

PARSE_FUNC(uint16);


#endif