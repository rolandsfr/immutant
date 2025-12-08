
#include "parse_unary.h"

#include "unity.h"

#include "ast_cnstrct.h"
#include "ast_expr.h"
#include "ast_make_expr.h"
#include "lexer.h" // TODO: remove after decoupled
#include "parse_comparison.h"
#include "parse_eq.h"
#include "parse_factor.h"
#include "parse_lassoc.h"
#include "parse_primary.h"
#include "parse_term.h"
#include "parser_helpers.h"
#include "resolve.h" // TODO: remove after decoupled
#include "test_expr.h"

void test_parse_unary(void)
{
	TokenBuffer tokens;
	init_token_buffer(&tokens);

	// Simulate tokens for the expression: a == b
	add_token(&tokens, create_token(TOKEN_MINUS, "-", 1, 1));
	add_token(&tokens, create_token(TOKEN_NUMBER, "2", 1, 1));

	size_t pos = 0;
	ErrorCode error = NO_ERROR;

	Expr* res = parse_primary(&tokens, &pos, &error);

	TEST_ASSERT_NOT_NULL(res);
	TEST_ASSERT_EQUAL_INT(EXPR_UNARY, res->type);
	TEST_ASSERT_EQUAL_STRING(EXPR_LITERAL_NUMBER,
							 ((UnaryExpr*)res)->operand->type);

	TEST_ASSERT_EQUAL_INT(TOKEN_MINUS, ((UnaryExpr*)res)->operator);

	free_token_buffer(&tokens);
}

void test_parse_unary_should_allow_double_unary(void)
{
	TokenBuffer tokens;
	init_token_buffer(&tokens);

	// Simulate tokens for the expression: --2
	add_token(&tokens, create_token(TOKEN_MINUS, "-", 1, 1));
	add_token(&tokens, create_token(TOKEN_MINUS, "-", 1, 1));
	add_token(&tokens, create_token(TOKEN_NUMBER, "2", 1, 1));

	size_t pos = 0;
	ErrorCode error = NO_ERROR;

	Expr* res = parse_unary(&tokens, &pos, &error);

	TEST_ASSERT_NOT_NULL(res);
	TEST_ASSERT_EQUAL_INT(EXPR_UNARY, res->type);
	UnaryExpr* first_unary = (UnaryExpr*)res;
	TEST_ASSERT_EQUAL_INT(TOKEN_MINUS, first_unary->operator);

	TEST_ASSERT_EQUAL_INT(EXPR_UNARY, first_unary->operand->type);
	UnaryExpr* second_unary = (UnaryExpr*)first_unary->operand;
	TEST_ASSERT_EQUAL_INT(TOKEN_MINUS, second_unary->operator);

	TEST_ASSERT_EQUAL_INT(EXPR_LITERAL_NUMBER, second_unary->operand->type);

	free_token_buffer(&tokens);
}
