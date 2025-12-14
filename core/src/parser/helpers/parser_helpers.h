#pragma once

#include <stdarg.h>

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

/** same as match_token, but accepts array rather than variadic list */
int match_any_token(TokenBuffer* tokens, size_t* pos,
					const enum TokenType* types, size_t type_count);

/** consumes and returns the token at the current position.
 * Advances position if token was consumed **/
Token consume_token(TokenBuffer* tokens, size_t* pos);

/** returns the type of the token at the previous position **/
enum TokenType previous_token(TokenBuffer* tokens, size_t pos);

/** returns the type of the token at the current position without consuming it
 * **/
enum TokenType peek_token(TokenBuffer* tokens, size_t pos);

Token prev_token(TokenBuffer* tokens, size_t pos);

Token peek_token_full(TokenBuffer* tokens, size_t pos);
