#pragma once

#include "parser_api.h"
#include "test_expr.h"
#include "token_buffer.h"

Token create_token(enum TokenType type, const char* lexeme, size_t length,
				   int line);
void add_token(TokenBuffer* buffer, Token token);
void init_token_buffer(TokenBuffer* buffer);
void free_token_buffer(TokenBuffer* buffer);

Expr* init_test_parse(TokenBuffer* tokens, int token_count,
					  SampleToken sample_tokens[], Error* error, ParseFn parse);
Stmt* init_test_parse_stmt(TokenBuffer* tokens, int token_count,
						   SampleToken sample_tokens[], Error* error,
						   ParseStmtFn parse);
