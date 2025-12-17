#pragma once

#include "parse_if_stmt.h"

#include "unity.h"

#include "ast_make_expr.h"
#include "ast_make_stmt.h"
#include "ast_stmt.h"
#include "lexer.h"
#include "parser_helpers.h"
#include "resolve.h"
#include "test_expr.h"

#include "Mockparse_dclr.h"
#include "Mockparse_expr.h"

Expr* mock_parse_expr(TokenBuffer* tokens, size_t* pos, Error* out_error,
					  int ignore)
{
	(*pos)++;
	return (Expr*)make_boolean_expr(1);
}

Stmt* mock_parse_dclr(TokenBuffer* tokens, size_t* pos, Error* out_error,
					  int num_calls)
{

	while (!is_at_end(tokens, *pos) &&
		   !check_token(tokens, *pos, TOKEN_RIGHT_BRACE)) {
		(*pos)++;
	}

	(*pos)++;

	if (num_calls == 0) { // first block (then branch)
		return (Stmt*)make_expr_stmt((Expr*)make_number_expr("42"));
	} else { // second block (else branch)
		return (Stmt*)make_expr_stmt((Expr*)make_number_expr("0"));
	}
}

void test_parse_if_stmt_should_get_if_else_blocks(void)
{
	TokenBuffer tokens;
	init_token_buffer(&tokens);
	Error error;
	size_t pos = 0;

	add_token(&tokens, create_token(TOKEN_LEFT_PAREN, "(", 1, 1));
	add_token(&tokens, create_token(TOKEN_TRUE, "true", 1, 2));
	add_token(&tokens, create_token(TOKEN_RIGHT_PAREN, ")", 1, 6));
	add_token(&tokens, create_token(TOKEN_LEFT_BRACE, "{", 1, 8));
	add_token(&tokens, create_token(TOKEN_RIGHT_BRACE, "}", 1, 10));
	add_token(&tokens, create_token(TOKEN_ELSE, "else", 1, 12));
	add_token(&tokens, create_token(TOKEN_LEFT_BRACE, "{", 1, 17));
	add_token(&tokens, create_token(TOKEN_RIGHT_BRACE, "}", 1, 19));

	parse_expr_StubWithCallback(mock_parse_expr);
	parse_dclr_StubWithCallback(mock_parse_dclr);

	IfStmt* if_stmt = parse_if_stmt(&tokens, &pos, &error);

	TEST_ASSERT_NOT_NULL(if_stmt);
	TEST_ASSERT_NOT_NULL(if_stmt->condition);
	TEST_ASSERT_LITERAL_BOOL_EXPR(if_stmt->condition, 1);

	Expr* then_expr = ((ExprStmt*)if_stmt->then_branch)->expression;
	TEST_ASSERT_LITERAL_NUMBER_EXPR(then_expr, "42");

	Expr* else_expr = ((ExprStmt*)if_stmt->else_branch)->expression;
	TEST_ASSERT_LITERAL_NUMBER_EXPR(else_expr, "0");

	free_token_buffer(&tokens);
}

void test_if_without_else(void)
{
	TokenBuffer tokens;
	init_token_buffer(&tokens);
	Error error = {0};
	size_t pos = 0;

	add_token(&tokens, create_token(TOKEN_LEFT_PAREN, "(", 1, 1));
	add_token(&tokens, create_token(TOKEN_TRUE, "true", 1, 2));
	add_token(&tokens, create_token(TOKEN_RIGHT_PAREN, ")", 1, 6));
	add_token(&tokens, create_token(TOKEN_LEFT_BRACE, "{", 1, 8));
	add_token(&tokens, create_token(TOKEN_RIGHT_BRACE, "}", 1, 9));

	parse_expr_StubWithCallback(mock_parse_expr);
	parse_dclr_StubWithCallback(mock_parse_dclr);

	IfStmt* stmt = parse_if_stmt(&tokens, &pos, &error);

	TEST_ASSERT_NOT_NULL(stmt);
	TEST_ASSERT_NULL(stmt->else_branch);

	free_token_buffer(&tokens);
}

void test_missing_left_paren(void)
{
	TokenBuffer tokens;
	init_token_buffer(&tokens);
	Error error = {0};
	size_t pos = 0;

	add_token(&tokens, create_token(TOKEN_TRUE, "true", 1, 1));

	IfStmt* stmt = parse_if_stmt(&tokens, &pos, &error);

	TEST_ASSERT_NULL(stmt);
	TEST_ASSERT_EQUAL(SYNTAX_ERROR_UNEXPECTED_TOKEN, error.type);

	free_token_buffer(&tokens);
}

void test_missing_right_paren(void)
{
	TokenBuffer tokens;
	init_token_buffer(&tokens);
	Error error = {0};
	size_t pos = 0;

	add_token(&tokens, create_token(TOKEN_LEFT_PAREN, "(", 1, 1));
	add_token(&tokens, create_token(TOKEN_TRUE, "true", 1, 2));

	parse_expr_StubWithCallback(mock_parse_expr);

	IfStmt* stmt = parse_if_stmt(&tokens, &pos, &error);

	TEST_ASSERT_NULL(stmt);
	TEST_ASSERT_EQUAL(SYNTAX_ERROR_UNEXPECTED_TOKEN, error.type);

	free_token_buffer(&tokens);
}

void test_condition_parse_failure(void)
{
	TokenBuffer tokens;
	init_token_buffer(&tokens);
	Error error = {0};
	size_t pos = 0;

	add_token(&tokens, create_token(TOKEN_LEFT_PAREN, "(", 1, 1));

	parse_expr_IgnoreAndReturn(NULL);

	IfStmt* stmt = parse_if_stmt(&tokens, &pos, &error);

	TEST_ASSERT_NULL(stmt);

	free_token_buffer(&tokens);
}
