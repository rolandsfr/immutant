
#include "parse_primary.h"

#include "unity.h"

#include "ast_cnstrct.h"
#include "ast_expr.h"
#include "ast_make_expr.h"
#include "lexer.h" // TODO: remove after decoupled
#include "parse_comparison.h"
#include "parse_eq.h"
#include "parse_factor.h"
#include "parse_lassoc.h"
#include "parse_term.h"
#include "parse_unary.h"
#include "parser_helpers.h"
#include "resolve.h" // TODO: remove after decoupled
#include "test_expr.h"

void test_parse_primary_should_parse_literal_values(void)
{
	TokenBuffer tokens;
	init_token_buffer(&tokens);

	// Simulate tokens for the expression: a == b
	add_token(&tokens, create_token(TOKEN_NUMBER, "2", 1, 1));

	size_t pos = 0;

	Expr* res = parse_primary(&tokens, &pos);

	TEST_ASSERT_NOT_NULL(res);
	TEST_ASSERT_EQUAL_INT(EXPR_LITERAL_NUMBER, res->type);
	TEST_ASSERT_EQUAL_STRING("2", ((NumberExpr*)res)->value);

	free_token_buffer(&tokens);
}

void test_parse_primary_should_parse_boolean_values(void)
{
	TokenBuffer tokens;
	init_token_buffer(&tokens);

	// Simulate tokens for the expression: true
	add_token(&tokens, create_token(TOKEN_TRUE, "true", 4, 1));

	size_t pos = 0;

	Expr* res = parse_primary(&tokens, &pos);

	TEST_ASSERT_NOT_NULL(res);
	TEST_ASSERT_EQUAL_INT(EXPR_LITERAL_BOOL, res->type);
	TEST_ASSERT_EQUAL_INT(1, ((BooleanExpr*)res)->value);

	free_token_buffer(&tokens);
}
