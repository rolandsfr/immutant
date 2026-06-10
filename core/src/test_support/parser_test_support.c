#include "parser_test_support.h"

#include "lexer.h"

Expr* init_test_parse(TokenBuffer* tokens, int token_count,
					  SampleToken sample_tokens[], Error* error, ParseFn parse)
{
	init_token_buffer(tokens);
	for (int i = 0; i < token_count; i++) {
		add_token(tokens,
				  create_token(sample_tokens[i].type, sample_tokens[i].lexeme,
							   sample_tokens[i].length, 1));
	}

	size_t pos = 0;
	*error = (Error){-1};

	return parse(tokens, &pos, error);
}

Stmt* init_test_parse_stmt(TokenBuffer* tokens, int token_count,
						   SampleToken sample_tokens[], Error* error,
						   ParseStmtFn parse)
{
	init_token_buffer(tokens);
	for (int i = 0; i < token_count; i++) {
		add_token(tokens,
				  create_token(sample_tokens[i].type, sample_tokens[i].lexeme,
							   sample_tokens[i].length, 1));
	}

	size_t pos = 0;
	*error = (Error){-1};

	return parse(tokens, &pos, error);
}
