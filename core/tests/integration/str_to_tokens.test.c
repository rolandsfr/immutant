#include "unity.h"

#include "ast_expr.h"
#include "ast_make_expr.h"
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

void test_should_convert_string_to_tokens()
{
	char* input = "1 + 2 * (5 - -3)";

	TokenBuffer tokens;
	init_token_buffer(&tokens);
	size_t pos = 0, line = 1;

	scan_tokens(input, &line, &tokens, &pos);

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

	ErrorReport error = init_no_error_report();
	Expr* ast = parse_equality(&tokens, &pos, &error);

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

void test_should_throw_on_unexpected_char()
{
	char* input = "1 + 2 * ($ - -3)";

	TokenBuffer tokens;
	init_token_buffer(&tokens);
	size_t pos = 0, line = 1;

	scan_tokens(input, &line, &tokens, &pos);

	TEST_ASSERT_EQUAL_INT(10, tokens.count);

	pos = 0;

	ErrorReport error = init_no_error_report();
	Expr* ast = parse_equality(&tokens, &pos, &error);
	TEST_ASSERT_NULL(ast);
	TEST_ASSERT_EQUAL_INT(ERROR_INVALID_TOKEN, error.code);

	free_token_buffer(&tokens);
}
