#include "parse_eq.h"

#include "unity.h"

#include "Mockparse_comparison.h"
#include "ast_make_expr.h"
#include "lexer.h" // TODO: remove after decoupled
#include "parser_helpers.h"
#include "resolve.h" // TODO: remove after decoupled
#

void test_parse_equality(void)
{
	TokenBuffer tokens;
	init_token_buffer(&tokens);

	// Simulate tokens for the expression: a == b
	add_token(&tokens, create_token(TOKEN_IDENTIFIER, "a", 1, 1));
	add_token(&tokens, create_token(TOKEN_EQUAL_EQUAL, "==", 2, 1));
	add_token(&tokens, create_token(TOKEN_IDENTIFIER, "b", 1, 1));
	add_token(&tokens, create_token(TOKEN_EOF, "", 0, 1));

	size_t pos = 0;
}
