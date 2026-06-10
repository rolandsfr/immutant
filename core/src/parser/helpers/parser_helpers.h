#pragma once

#include <stdarg.h>

#include "lexer.h"

int is_at_end(TokenBuffer* tokens, size_t pos);
int check_token(TokenBuffer* tokens, size_t pos, enum TokenType type);
int match_token(TokenBuffer* tokens, size_t* pos, int count, ...);
int match_any_token(TokenBuffer* tokens, size_t* pos,
					const enum TokenType* types, size_t type_count);
Token consume_token(TokenBuffer* tokens, size_t* pos);
enum TokenType previous_token(TokenBuffer* tokens, size_t pos);
enum TokenType peek_token(TokenBuffer* tokens, size_t pos);
Token prev_token(TokenBuffer* tokens, size_t pos);
Token peek_token_full(TokenBuffer* tokens, size_t pos);
