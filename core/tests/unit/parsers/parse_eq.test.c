#include "parse_eq.h"

#include "unity.h"

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
#include "parse_primary.h"
#include "parse_term.h"
#include "parse_unary.h"
#include "parser_helpers.h"
#include "parser_singnature.h"
#include "resolve.h" // TODO: remove after decoupled
#include "test_expr.h"

void test_parse_eq_should_parse_equality_expression(void)
{
	TokenBuffer tokens;
	ErrorReport error;
	Expr* res = init_test_parse(&tokens, 3,
								(SampleToken[]){
									{TOKEN_NUMBER, "2", 1},
									{TOKEN_EQUAL_EQUAL, "==", 2},
									{TOKEN_NUMBER, "7", 1},
								},
								&error, parse_equality);

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

void test_parse_eq_should_exit_immediately_on_parse_error(void)
{

	TokenBuffer tokens;
	ErrorReport error;

	Expr* res = init_test_parse(&tokens, 2,
								(SampleToken[]){
									{TOKEN_NUMBER, "2", 1},
									{TOKEN_EQUAL_EQUAL, "==", 2},
									// missing right-hand side operand
								},
								&error, parse_equality);

	TEST_ASSERT_NULL(res);
	TEST_ASSERT_EQUAL_INT(ERROR_UNEXPECTED_TOKEN, error.code);

	free_token_buffer(&tokens);
}
