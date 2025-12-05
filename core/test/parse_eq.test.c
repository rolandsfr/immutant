#include "parse_eq.h"

#include "unity.h"

#include "Mockparse_comparison.h"
#include "ast_cnstrct.h"
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

	size_t pos = 0;

	Expr* left = (Expr*)make_variable_expr("a");
	Expr* right = (Expr*)make_variable_expr("b");
	pos++;

	parse_comparison_ExpectAndReturn(&tokens, &pos, left);
	parse_comparison_ExpectAndReturn(&tokens, &pos, right);

	Expr* res = (Expr*)parse_equality(&tokens, &pos);

	// Verify the result is a binary expression
	TEST_ASSERT_NOT_NULL(res);
	TEST_ASSERT_EQUAL(EXPR_BINARY, res->type);
}
