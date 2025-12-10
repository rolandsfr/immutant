
#pragma once

#include <stdarg.h>

#include "ast_cnstrct.h"
#include "lexer.h"

/**
 *
 * ===================================
 * Utils for navigation through tokens
 * ===================================
 *
 **/

/** returns boolean indicating if the current position is at the end of the
 * token buffer **/
int is_at_end(TokenBuffer* tokens, size_t pos);

/** returns boolean indicating if the token at the current position matches the
 * given type **/
int check_token(TokenBuffer* tokens, size_t pos, enum TokenType type);

/** tries to match the token at the current position against a list of types
 *  if a match is found, consumes the token and returns true
 *  otherwise, returns false **/
int match_token(TokenBuffer* tokens, size_t* pos, int count, ...);

/** consumes and returns the token at the current position.
 * Advances position if token was consumed **/
Token consume_token(TokenBuffer* tokens, size_t* pos);

/**
 *
 * ==============================================
 * Parsers for each grammer production rule (LR)
 * ==============================================
 *
 **/

/** parses comparison for production rule:
 *
 * comparison -> addition ( ( ">" | ">=" | "<" | "<=" ) addition )*
 * */
Expr* parse_comparison(TokenBuffer* tokens, size_t* pos);

/** parses equality for production rule:
 *
 * equality -> comparison ( ( "!=" | "==" ) comparison )*
 *
 * */
Expr* parse_equality(TokenBuffer* tokens, size_t* pos);
