#include "parse_eq.h"

#include "unity.h"

#include "ast_expr.h"
#include "ast_make_expr.h"
#include "lexer.h" // TODO: remove after decoupled
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

void test_parse_eq_should_parse_equality_expression(void)
{
	TokenBuffer tokens;
	init_token_buffer(&tokens);

	// Simulate tokens for the expression:
	add_token(&tokens, create_token(TOKEN_NUMBER, "2", 1, 1));
	add_token(&tokens, create_token(TOKEN_EQUAL_EQUAL, "==", 1, 1));
	add_token(&tokens, create_token(TOKEN_NUMBER, "4", 1, 1));

	size_t pos = 0;
	ErrorCode error = NO_ERROR;

	Expr* res = parse_equality(&tokens, &pos, &error);

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
	TEST_ASSERT_LITERAL_NUMBER_EXPR(binary_expr->right, "4");

	free_token_buffer(&tokens);
}
