#include "parse_term.h"

#include "unity.h"

#include "ast_cnstrct.h"
#include "ast_expr.h"
#include "ast_make_expr.h"
#include "error.h"
#include "error_report.h"
#include "lexer.h" // TODO: remove after decoupled
#include "parse_comparison.h"
#include "parse_eq.h"
#include "parse_factor.h"
#include "parse_lassoc.h"
#include "parse_primary.h"
#include "parse_unary.h"
#include "parser_helpers.h"
#include "resolve.h" // TODO: remove after decoupled
#include "test_expr.h"

void test_parse_term(void)
{
	TokenBuffer tokens;
	ErrorReport error;
	Expr* res = init_test_parse(&tokens, 5,
								(SampleToken[]){
									{TOKEN_NUMBER, "2", 1},
									{TOKEN_PLUS, "+", 1},
									{TOKEN_NUMBER, "4", 1},
									{TOKEN_SLASH, "/", 1},
									{TOKEN_NUMBER, "5", 1},
								},
								&error, parse_term);

	/** AST being asserted:
	 *
	 *         (+)
	 *        /   \
	 *      2     (/)
	 *           /   \
	 *          4     5
	 *
	 *  */

	TEST_ASSERT_NOT_NULL(res);
	BinaryExpr* binary_expr = (BinaryExpr*)res;
	TEST_ASSERT_BINARY_EXPR(binary_expr, TOKEN_PLUS);
	TEST_ASSERT_LITERAL_NUMBER_EXPR(binary_expr->left, "2");
	BinaryExpr* right_expr = (BinaryExpr*)binary_expr->right;
	TEST_ASSERT_BINARY_EXPR(right_expr, TOKEN_SLASH);
	TEST_ASSERT_LITERAL_NUMBER_EXPR(right_expr->left, "4");
	TEST_ASSERT_LITERAL_NUMBER_EXPR(right_expr->right, "5");

	free_token_buffer(&tokens);
}
