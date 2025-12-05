#include "parser_helpers.h"

#include <stdarg.h>

int sum(int a, int b) { return a + b; }

int is_at_end(TokenBuffer* tokens, size_t pos)
{
	return pos >= tokens->count || tokens->tokens[pos].type == TOKEN_EOF;
}

int check_token(TokenBuffer* tokens, size_t pos, enum TokenType type)
{
	if (is_at_end(tokens, pos)) {

		return 0;
	}
	return tokens->tokens[pos].type == type;
}

int match_token(TokenBuffer* tokens, size_t* pos, int count, ...)
{
	va_list args;
	va_start(args, count);

	for (int i = 0; i < count; i++) {
		enum TokenType type = va_arg(args, enum TokenType);
		if (check_token(tokens, *pos, type)) {
			consume_token(tokens, pos);
			va_end(args);
			return 1;
		}
	}

	va_end(args);
	return 0;
}

Token consume_token(TokenBuffer* tokens, size_t* pos)
{
	if (!is_at_end(tokens, *pos)) {
		(*pos)++;
	}

	Token token = tokens->tokens[*pos - 1];
	return token;
}
