
#include "parse_lassoc.h"

#include "unity.h"

#include "ast_cnstrct.h"
#include "ast_expr.h"
#include "ast_make_expr.h"
#include "error.h"
#include "error_report.h"
#include "lexer.h" // TODO: remove after decoupled
#include "parser_helpers.h"
#include "parser_singnature.h"
#include "resolve.h" // TODO: remove after decoupled
#include "test_expr.h"

// always consumes one token and returns a variable expression
static DEF_PARSE_FN(dummy_parse)
{
	Token t = tokens->tokens[*pos];
	(*pos)++;
	return (Expr*)make_variable_expr(t.lexeme, t.line);
}

void test_parse_lassoc_should_parse_left_associativity_expression(void)
{
	TokenBuffer tokens;
	init_token_buffer(&tokens);

	// Simulate tokens for the expression: a == b
	add_token(&tokens, create_token(TOKEN_IDENTIFIER, "a", 1, 1));
	add_token(&tokens, create_token(TOKEN_EQUAL_EQUAL, "==", 2, 1));
	add_token(&tokens, create_token(TOKEN_IDENTIFIER, "b", 1, 1));
	add_token(&tokens, create_token(TOKEN_GREATER_EQUAL, ">=", 2, 1));
	add_token(&tokens, create_token(TOKEN_IDENTIFIER, "c", 1, 1));

	size_t pos = 0;
	Error error = {-1};

	Expr* first = (Expr*)make_variable_expr("a", 1);
	Expr* second = (Expr*)make_variable_expr("b", 1);
	Expr* third = (Expr*)make_variable_expr("c", 1);

	const enum TokenType operators[] = {
		TOKEN_EQUAL_EQUAL,
		TOKEN_GREATER_EQUAL}; // random example operators, not
							  // actually part of the grammar

	const size_t operator_count = sizeof(operators) / sizeof(operators[0]);

	Expr* res = parse_lassoc(&tokens, &pos, dummy_parse, operators,
							 operator_count, &error);

	/** Test for correct AST:

				>=
			   /  \
			 ==    c
			/  \
		   a    b

	*/

	TEST_ASSERT_NOT_NULL(res);
	TEST_ASSERT_EQUAL(EXPR_BINARY, res->type);

	BinaryExpr* outer_binary =
		(BinaryExpr*)res; // binary for outer most expression ((a == b) >= c)

	TEST_ASSERT_EQUAL_INT(TOKEN_GREATER_EQUAL, outer_binary->operator);
	TEST_ASSERT_VAR_EXPR(outer_binary->right, "c");

	// inner binary for (a == b) which is left most
	BinaryExpr* inner_binary = (BinaryExpr*)outer_binary->left;
	TEST_ASSERT_EQUAL_INT(TOKEN_EQUAL_EQUAL, inner_binary->operator);
	TEST_ASSERT_VAR_EXPR(inner_binary->left, "a");
	TEST_ASSERT_VAR_EXPR(inner_binary->right, "b");

	free_token_buffer(&tokens);
}
