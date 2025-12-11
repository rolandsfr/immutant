
#include "parse_primary.h"

#include "unity.h"

#include "ast_cnstrct.h"
#include "ast_expr.h"
#include "ast_make_expr.h"
#include "error.h"
#include "error_codes.h"
#include "error_report.h"
#include "lexer.h" // TODO: remove after decoupled
#include "parse_comparison.h"
#include "parse_eq.h"
#include "parse_factor.h"
#include "parse_lassoc.h"
#include "parse_term.h"
#include "parse_unary.h"
#include "parser_helpers.h"
#include "parser_singnature.h"
#include "resolve.h" // TODO: remove after decoupled
#include "test_expr.h"

void test_parse_primary_should_parse_literal_values(void)
{
	TokenBuffer tokens;
	ErrorReport error;
	Expr* res = init_test_parse(&tokens, 1,
								(SampleToken[]){
									{TOKEN_NUMBER, "2", 1},
								},
								&error, parse_primary);

	TEST_ASSERT_NOT_NULL(res);
	TEST_ASSERT_EQUAL_INT(EXPR_LITERAL_NUMBER, res->type);
	TEST_ASSERT_EQUAL_STRING("2", ((NumberExpr*)res)->value);

	free_token_buffer(&tokens);
}

void test_parse_primary_should_parse_boolean_values(void)
{
	TokenBuffer tokens;
	ErrorReport error;

	Expr* res = init_test_parse(&tokens, 1,
								(SampleToken[]){
									{TOKEN_TRUE, "true", 4},
								},
								&error, parse_primary);

	TEST_ASSERT_NOT_NULL(res);
	TEST_ASSERT_EQUAL_INT(EXPR_LITERAL_BOOL, res->type);
	TEST_ASSERT_EQUAL_INT(1, ((BooleanExpr*)res)->value);

	free_token_buffer(&tokens);
}

void test_parse_primary_should_parse_parenthesized_expression(void)
{
	TokenBuffer tokens;
	ErrorReport error;

	// Expr* res = parse_primary(&tokens, &pos, &error);
	Expr* res = init_test_parse(&tokens, 5,
								(SampleToken[]){
									{TOKEN_LEFT_PAREN, "(", 1},
									{TOKEN_NUMBER, "8", 1},
									{TOKEN_PLUS, "+", 1},
									{TOKEN_NUMBER, "9", 1},
									{TOKEN_RIGHT_PAREN, ")", 1},
								},
								&error, parse_primary);

	/** AST being asserted:
	 *
	 *        (+)
	 *       /   \
	 *      2     3
	 *
	 *  */

	TEST_ASSERT_NOT_NULL(res);
	BinaryExpr* binary_expr = (BinaryExpr*)res;
	TEST_ASSERT_BINARY_EXPR(binary_expr, TOKEN_PLUS);
	TEST_ASSERT_LITERAL_NUMBER_EXPR(binary_expr->left, "8");
	TEST_ASSERT_LITERAL_NUMBER_EXPR(binary_expr->right, "9");

	free_token_buffer(&tokens);
}

void test_parse_primary_should_detect_missing_closing_parenthesis(void)
{
	TokenBuffer tokens;
	ErrorReport error;

	Expr* res = init_test_parse(&tokens, 3,
								(SampleToken[]){
									{TOKEN_LEFT_PAREN, "(", 1},
									{TOKEN_NUMBER, "2", 1},
									{TOKEN_PLUS, "+", 1},
								},
								&error, parse_primary);

	TEST_ASSERT_NULL(res);
	TEST_ASSERT_EQUAL_INT(ERROR_UNEXPECTED_TOKEN, error.code);

	free_token_buffer(&tokens);
}

void test_parse_primary_should_detect_unexpected_token(void)
{
	TokenBuffer tokens;
	ErrorReport error;

	Expr* res = init_test_parse(&tokens, 1,
								(SampleToken[]){
									{TOKEN_UNRECOGNIZED, "@", 1},
								},
								&error, parse_primary);

	TEST_ASSERT_NULL(res);
	TEST_ASSERT_EQUAL_INT(ERROR_INVALID_TOKEN, error.code);

	free_token_buffer(&tokens);
}
