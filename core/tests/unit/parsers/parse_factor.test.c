#include "parse_factor.h"

#include "unity.h"

#include "ast_cnstrct.h"
#include "ast_expr.h"
#include "ast_make_expr.h"
#include "error.h"
#include "error_report.h"
#include "lexer.h" // TODO: remove after decoupled
#include "parse_comparison.h"
#include "parse_eq.h"
#include "parse_lassoc.h"
#include "parse_primary.h"
#include "parse_term.h"
#include "parse_unary.h"
#include "parser_helpers.h"
#include "resolve.h" // TODO: remove after decoupled
#include "test_expr.h"

void test_parse_factor(void)
{
	TokenBuffer tokens;
	ErrorReport error;

	BinaryExpr* res = (BinaryExpr*)init_test_parse(&tokens, 5,
												   (SampleToken[]){
													   {TOKEN_NUMBER, "2", 1},
													   {TOKEN_SLASH, "/", 1},
													   {TOKEN_NUMBER, "3", 1},
													   {TOKEN_STAR, "*", 1},
													   {TOKEN_NUMBER, "66", 2},
												   },
												   &error, parse_factor);

	BinaryExpr* binary_expr = (BinaryExpr*)res;
	BinaryExpr* left_expr = (BinaryExpr*)binary_expr->left;

	/** AST being asserted:
	 *
	 *         (*)
	 *        /   \
	 *      (/)   66
	 *     /   \
	 *    2     3
	 *
	 */

	TEST_ASSERT_NOT_NULL(res);
	TEST_ASSERT_BINARY_EXPR(res, TOKEN_STAR);
	TEST_ASSERT_BINARY_EXPR(left_expr, TOKEN_SLASH);
	TEST_ASSERT_LITERAL_NUMBER_EXPR(left_expr->left, "2");
	TEST_ASSERT_LITERAL_NUMBER_EXPR(left_expr->right, "3");
	TEST_ASSERT_LITERAL_NUMBER_EXPR(binary_expr->right, "66");

	free_token_buffer(&tokens);
}
