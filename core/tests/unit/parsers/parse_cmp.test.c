#include "unity.h"

#include "ast_cnstrct.h"
#include "ast_expr.h"
#include "ast_make_expr.h"
#include "error.h"
#include "error_codes.h"
#include "error_report.h"
#include "lexer.h" // TODO: remove after decoupled
#include "parse_asgn.h"
#include "parse_comparison.h"
#include "parse_eq.h"
#include "parse_factor.h"
#include "parse_lassoc.h"
#include "parse_primary.h"
#include "parse_term.h"
#include "parse_unary.h"
#include "parser_helpers.h"
#include "resolve.h" // TODO: remove after decoupled
#include "test_expr.h"

#include "Mockparse_or.h"

void test_cmp(void)
{
	TokenBuffer tokens;
	Error error;

	Expr* res = init_test_parse(&tokens, 5,
								(SampleToken[]){
									{TOKEN_NUMBER, "2", 1},
									{TOKEN_SLASH, "/", 1},
									{TOKEN_NUMBER, "4", 1},
									{TOKEN_GREATER, ">", 1},
									{TOKEN_NUMBER, "5", 1},
								},
								&error, parse_comparison);

	/** AST being asserted:
	 *
	 *        (>)
	 *       /   \
	 *     (/)    5
	 *    /   \
	 *   2     4
	 *
	 *  */

	TEST_ASSERT_NOT_NULL(res);
	BinaryExpr* binary_expr = (BinaryExpr*)res;
	TEST_ASSERT_BINARY_EXPR(binary_expr, TOKEN_GREATER);
	TEST_ASSERT_LITERAL_NUMBER_EXPR(binary_expr->right, "5");
	BinaryExpr* left_expr = (BinaryExpr*)binary_expr->left;
	TEST_ASSERT_BINARY_EXPR(left_expr, TOKEN_SLASH);
	TEST_ASSERT_LITERAL_NUMBER_EXPR(left_expr->left, "2");
	TEST_ASSERT_LITERAL_NUMBER_EXPR(left_expr->right, "4");

	free_token_buffer(&tokens);
}

void test_cmp_should_exit_immediately_on_parse_error(void)
{

	TokenBuffer tokens;
	Error error;

	Expr* res = init_test_parse(&tokens, 4,
								(SampleToken[]){
									{TOKEN_NUMBER, "2", 1},
									{TOKEN_SLASH, "/", 1},
									{TOKEN_NUMBER, "4", 1},
									{TOKEN_GREATER, ">", 1},
								},
								&error, parse_comparison);

	TEST_ASSERT_NULL(res);
	TEST_ASSERT_EQUAL_INT(SYNTAX_ERROR_MISSING_EXPRESSION, error.type);
	TEST_ASSERT_EQUAL_STRING("Expected expression", error.message);
	TEST_ASSERT_EQUAL_INT(1, error.line);

	free_token_buffer(&tokens);
}
