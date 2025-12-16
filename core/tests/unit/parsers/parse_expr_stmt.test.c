#include "parse_expr_stmt.h"

#include "unity.h"

#include "assert_value.h"
#include "ast_cnstrct.h"
#include "ast_make_expr.h"
#include "ast_make_stmt.h"
#include "ast_stmt.h"
#include "error.h"
#include "lexer.h"
#include "Mockparse_asgn.h"
#include "parser_helpers.h"
#include "test_expr.h"

#include "Mockparse_expr.h"
#include "Mockresolve.h"

void test_parse_expr_stmt_should_fail_if_missing_semicolon_after_statement_expr(
	void)
{
	TokenBuffer tokens;
	Error error = {-1};
	init_token_buffer(&tokens);
	size_t pos = 0;

	add_token(&tokens, create_token(TOKEN_NUMBER, "5", 1, 1));
	add_token(&tokens, create_token(TOKEN_PLUS, "+", 1, 2));
	add_token(&tokens, create_token(TOKEN_NUMBER, "3", 1, 3));

	BinaryExpr* binary_expr = make_binary_expr(
		(Expr*)make_number_expr("5"), TOKEN_PLUS, (Expr*)make_number_expr("3"));

	parse_expr_ExpectAndReturn(&tokens, &pos, &error, (Expr*)binary_expr);

	ExprStmt* res = parse_expr_stmt(&tokens, &pos, &error);

	TEST_ASSERT_NULL(res);
	TEST_ASSERT_RUNTIME_ERROR(&error, SYNTAX_ERROR_UNEXPECTED_TOKEN);
	TEST_ASSERT_EQUAL_STRING(error.message,
							 "Expected ';' after expression statement.");

	free_token_buffer(&tokens);
}

void test_parse_expr_stmt_should_parse_expression_statement_with_semicolon(void)
{
	TokenBuffer tokens;
	Error error = {-1};
	init_token_buffer(&tokens);
	size_t pos = 0;

	add_token(&tokens, create_token(TOKEN_NUMBER, "10", 2, 1));
	add_token(&tokens, create_token(TOKEN_SEMICOLON, ";", 1, 2));

	NumberExpr* number_expr = make_number_expr("10");

	parse_expr_ExpectAndReturn(&tokens, &pos, &error, (Expr*)number_expr);
	pos++;

	ExprStmt* res = parse_expr_stmt(&tokens, &pos, &error);

	TEST_ASSERT_NOT_NULL(res);
	TEST_ASSERT_LITERAL_NUMBER_EXPR(((ExprStmt*)res)->expression, "10");
	free_expr(((ExprStmt*)res)->expression);
	free_token_buffer(&tokens);
}
