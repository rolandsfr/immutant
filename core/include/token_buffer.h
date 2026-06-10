#ifndef TOKEN_BUFFER_H
#define TOKEN_BUFFER_H

#include <stddef.h>

#include "token_types.h"

typedef struct Token {
	enum TokenType type;
	char* lexeme;
	size_t length;
	int line;
} Token;

typedef struct TokenBuffer {
	Token* tokens;
	size_t count;
	size_t capacity;
} TokenBuffer;

#endif
