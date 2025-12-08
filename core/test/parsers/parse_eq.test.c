#include "parse_eq.h"

#include "unity.h"

#include "ast_expr.h"
#include "ast_make_expr.h"
#include "lexer.h" // TODO: remove after decoupled
#include "parse_comparison.h"
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
	// TokenBuffer tokens;
	// init_token_buffer(&tokens);

	// // Simulate tokens for the expression: a == b
	// add_token(&tokens, create_token(TOKEN_IDENTIFIER, "a", 1, 1));
	// add_token(&tokens, create_token(TOKEN_EQUAL_EQUAL, "==", 2, 1));
	// add_token(&tokens, create_token(TOKEN_IDENTIFIER, "b", 1, 1));

	// size_t pos = 0;

	// Expr* res = parse_equality(&tokens, &pos);

	// /** Test for correct AST:

	// 		==
	// 	   /  \
	// 	  a    b

	// */

	// TEST_ASSERT_NOT_NULL(res);
	// TEST_ASSERT_EQUAL_INT(EXPR_BINARY, res->type);

	// BinaryExpr* binary_expr = (BinaryExpr*)res;
	// TEST_ASSERT_EQUAL_INT(TOKEN_EQUAL_EQUAL, binary_expr->operator);

	// TEST_ASSERT_VAR_EXPR(binary_expr->left, "a");
	// TEST_ASSERT_VAR_EXPR(binary_expr->right, "b");

	// free_token_buffer(&tokens);
}
