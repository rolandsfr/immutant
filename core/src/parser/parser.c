#include "parser.h"

#include <stdarg.h>
#include <stdlib.h>

#include "ast_stmt.h"
#include "error.h"
#include "lexer.h"
#include "parse_stmt.h"
#include "parser_helpers.h"
#include "sync_parse.h"

Stmts parse(TokenBuffer* tokens, ErrorBuffer* out_errors)
{
	size_t pos = 0;

	Stmts stmts;
	Stmts_init(&stmts);

	while (!is_at_end(tokens, pos)) {
		Error error = {-1};
		Stmt* stmt = parse_dclr(tokens, &pos, &error);

		if (error.type != ERROR_NONE || stmt == NULL) {
			ErrorBuffer_push(out_errors, error);
			sync_parse(tokens, &pos, &error);
			continue;
		}

		Stmts_push(&stmts, stmt);
	}

	return stmts;
}

void sync_parse(TokenBuffer* tokens, size_t* pos, Error* out_error)
{
	while (!is_at_end(tokens, *pos)) {
		enum TokenType next_token = peek_token(tokens, *pos);

		switch (next_token) {
			case TOKEN_FN:
			case TOKEN_SEMICOLON:
			case TOKEN_MUTANT:
			case TOKEN_IMMUTANT:
			case TOKEN_IF:
			case TOKEN_WHILE:
			case TOKEN_RETURN: {
				if (out_error) {
					*out_error = (Error){.type = ERROR_NONE, .message = ""};
				}

				return;
			}
			default:
				break;
		}

		consume_token(tokens, pos);
	}
}

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
