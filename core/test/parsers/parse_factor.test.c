#include "parse_factor.h"

#include "unity.h"

#include "ast_cnstrct.h"
#include "ast_expr.h"
#include "ast_make_expr.h"
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
	init_token_buffer(&tokens);

	// Simulate tokens for the expression: a == b
	add_token(&tokens, create_token(TOKEN_NUMBER, "2", 1, 1));
	add_token(&tokens, create_token(TOKEN_SLASH, "/", 1, 1));
	add_token(&tokens, create_token(TOKEN_NUMBER, "3", 1, 1));
	add_token(&tokens, create_token(TOKEN_STAR, "*", 1, 1));
	add_token(&tokens, create_token(TOKEN_NUMBER, "66", 2, 1));

	size_t pos = 0;
	ErrorCode error = NO_ERROR;

	BinaryExpr* res = (BinaryExpr*)parse_factor(&tokens, &pos, &error);
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
