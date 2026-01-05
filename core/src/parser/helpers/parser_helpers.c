#include "parser_helpers.h"

#include <stdarg.h>

#include "lexer.h"

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

enum TokenType peek_token(TokenBuffer* tokens, size_t pos)
{
	if (is_at_end(tokens, pos)) {
		return TOKEN_EOF;
	}
	return tokens->tokens[pos].type;
}

Token peek_token_full(TokenBuffer* tokens, size_t pos)
{
	if (is_at_end(tokens, pos)) {
		return create_token(TOKEN_EOF, "", 0,
							tokens->tokens[tokens->count - 1].line);
	}
	return tokens->tokens[pos];
}

enum TokenType previous_token(TokenBuffer* tokens, size_t pos)
{
	if (pos == 0 || is_at_end(tokens, pos - 1)) {
		return TOKEN_EOF;
	}
	return tokens->tokens[pos - 1].type;
}

Token prev_token(TokenBuffer* tokens, size_t pos)
{
	if (pos == 0 || is_at_end(tokens, pos - 1)) {
		return tokens->tokens[tokens->count - 1];
	}
	return tokens->tokens[pos - 1];
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

int match_any_token(TokenBuffer* tokens, size_t* pos,
					const enum TokenType* types, size_t type_count)
{
	for (size_t i = 0; i < type_count; i++) {
		if (check_token(tokens, *pos, types[i])) {
			consume_token(tokens, pos);
			return 1;
		}
	}

	return 0;
}

Token consume_token(TokenBuffer* tokens, size_t* pos)
{
	if (is_at_end(tokens, *pos)) {
		return create_token(TOKEN_EOF, "", 0, 1);
	}

	(*pos)++;

	Token token = tokens->tokens[*pos - 1];
	return token;
}
