
#include "parse_expr.h"

#include "unity.h"

#include "ast_expr.h"
#include "ast_make_expr.h"
#include "error.h"
#include "error_codes.h"
#include "error_report.h"
#include "lexer.h" // TODO: remove after decoupled
#include "parse_and.h"
#include "parse_asgn.h"
#include "parse_comparison.h"
#include "parse_eq.h"
#include "parse_factor.h"
#include "parse_lassoc.h"
#include "parse_or.h"
#include "parse_primary.h"
#include "parse_term.h"
#include "parse_unary.h"
#include "parser_helpers.h"
#include "parser_singnature.h"
#include "resolve.h" // TODO: remove after decoupled
#include "test_expr.h"

void test_parse_expr_should_parse_equality_expression(void)
{
	TokenBuffer tokens;
	Error error = {-1};
	Expr* res = init_test_parse(&tokens, 3,
								(SampleToken[]){
									{TOKEN_NUMBER, "2", 1},
									{TOKEN_EQUAL_EQUAL, "==", 2},
									{TOKEN_NUMBER, "7", 1},
								},
								&error, parse_expr);

	/** AST being asserted:
	 *
	 *        (==)
	 *        /   \
	 *       2     4
	 *
	 *  */

	TEST_ASSERT_NOT_NULL(res);
	BinaryExpr* binary_expr = (BinaryExpr*)res;
	TEST_ASSERT_BINARY_EXPR(binary_expr, TOKEN_EQUAL_EQUAL);
	TEST_ASSERT_LITERAL_NUMBER_EXPR(binary_expr->left, "2");
	TEST_ASSERT_LITERAL_NUMBER_EXPR(binary_expr->right, "7");

	free_token_buffer(&tokens);
}

void test_parse_expr_should_parse_comparison_expression(void)
{
	TokenBuffer tokens;
	Error error = {-1};
	Expr* res = init_test_parse(&tokens, 3,
								(SampleToken[]){
									{TOKEN_NUMBER, "20", 2},
									{TOKEN_GREATER, ">", 2},
									{TOKEN_NUMBER, "7", 1},
								},
								&error, parse_expr);

	/** AST being asserted:
	 *
	 *        (==)
	 *        /   \
	 *       2     4
	 *
	 *  */

	TEST_ASSERT_NOT_NULL(res);
	BinaryExpr* binary_expr = (BinaryExpr*)res;
	TEST_ASSERT_BINARY_EXPR(binary_expr, TOKEN_GREATER);
	TEST_ASSERT_LITERAL_NUMBER_EXPR(binary_expr->left, "20");
	TEST_ASSERT_LITERAL_NUMBER_EXPR(binary_expr->right, "7");

	free_token_buffer(&tokens);
}

void test_parse_expr_should_exit_immediately_on_parse_error(void)
{

	TokenBuffer tokens;
	Error error = {-1};

	Expr* res = init_test_parse(&tokens, 2,
								(SampleToken[]){
									{TOKEN_NUMBER, "2", 1},
									{TOKEN_EQUAL_EQUAL, "==", 2},
								},
								&error, parse_equality);

	TEST_ASSERT_NULL(res);
	TEST_ASSERT_EQUAL_INT(SYNTAX_ERROR_MISSING_EXPRESSION, error.type);
	TEST_ASSERT_EQUAL_STRING("Expected expression", error.message);

	free_token_buffer(&tokens);
}

void test_parse_expr_should_parse_unary_expression(void)
{
	TokenBuffer tokens;
	Error error = {-1};
	Expr* res = init_test_parse(&tokens, 2,
								(SampleToken[]){
									{TOKEN_MINUS, "-", 1},
									{TOKEN_NUMBER, "4", 1},
								},
								&error, parse_expr);

	TEST_ASSERT_NOT_NULL(res);
	UnaryExpr* unary_expr = (UnaryExpr*)res;
	TEST_ASSERT_UNARY_NUMBER_XPR(res, TOKEN_MINUS, "4");

	free_token_buffer(&tokens);
}
