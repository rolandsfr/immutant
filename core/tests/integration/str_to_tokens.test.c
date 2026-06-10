#include "unity.h"

#include "assert_error.h"
#include "ast_expr.h"
#include "ast_make_expr.h"
#include "ast_make_stmt.h"
#include "error.h"
#include "error_codes.h"
#include "error_report.h"
#include "lexer.h" // TODO: remove after decoupled
#include "parse_expr.h"
#include "parse_stmt.h"
#include "parser.h"
#include "resolve.h"
#include "test_expr.h"

void test_should_convert_string_to_tokens()
{
	char* input = "1 + 2 * (5 - -3)";

	TokenBuffer tokens;
	init_token_buffer(&tokens);
	size_t pos = 0, line = 1;

	scan_tokens(input, &line, &tokens, &pos, NULL);

	TEST_ASSERT_EQUAL_INT(10, tokens.count);

	/** AST being asserted:

		 +
		/ \
	   1   *
		  / \
		 2   -
			/ \
		   5   -
			   |
			   3

	*/

	pos = 0;

	Error error = {-1};
	Expr* ast = parse_expr(&tokens, &pos, &error);

	TEST_ASSERT_BINARY_EXPR((BinaryExpr*)ast, TOKEN_PLUS);
	TEST_ASSERT_LITERAL_NUMBER_EXPR(((BinaryExpr*)ast)->left, "1");
	TEST_ASSERT_BINARY_EXPR((BinaryExpr*)((BinaryExpr*)ast)->right, TOKEN_STAR);
	TEST_ASSERT_LITERAL_NUMBER_EXPR(
		((BinaryExpr*)((BinaryExpr*)ast)->right)->left, "2");
	TEST_ASSERT_BINARY_EXPR(
		(BinaryExpr*)((BinaryExpr*)((BinaryExpr*)ast)->right)->right,
		TOKEN_MINUS);
	TEST_ASSERT_LITERAL_NUMBER_EXPR(
		((BinaryExpr*)((BinaryExpr*)((BinaryExpr*)ast)->right)->right)->left,
		"5");
	TEST_ASSERT_UNARY_NUMBER_XPR(
		((BinaryExpr*)((BinaryExpr*)((BinaryExpr*)ast)->right)->right)->right,
		TOKEN_MINUS, "3");

	free_token_buffer(&tokens);
}
