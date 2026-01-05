#include "parse_return_stmt.h"

#include "unity.h"

#include "ast_expr.h"
#include "ast_make_expr.h"
#include "ast_make_stmt.h"
#include "lexer.h"
#include "parser_helpers.h"
#include "test_expr.h"

#include "Mockparse_expr.h"
#include "Mockresolve.h"

Expr* mock_parse_expr(TokenBuffer* tokens, size_t* pos, Error* out_error,
					  int num_calls)
{

	if (num_calls == 0) {
		while (!is_at_end(tokens, *pos) &&
			   !check_token(tokens, *pos, TOKEN_SEMICOLON)) {
			(*pos)++;
		}

		Expr* expr = (Expr*)make_variable_expr("a", 2);
		return expr;
	} else {
		return NULL;
	}
}

void test_parse_return_stmt_with_value(void)
{
	TokenBuffer tokens;
	size_t pos = 0;
	Error error = {-1};

	init_token_buffer(&tokens);
	// add tokens
	add_token(&tokens, create_token(TOKEN_RETURN, "return", 6, 1));
	add_token(&tokens, create_token(TOKEN_IDENTIFIER, "a", 1, 2));
	add_token(&tokens, create_token(TOKEN_SEMICOLON, ";", 1, 3));

	parse_expr_StubWithCallback(mock_parse_expr);

	ReturnStmt* stmt = parse_return_stmt(&tokens, &pos, &error);

	TEST_ASSERT_NOT_NULL(stmt);
	TEST_ASSERT_VAR_EXPR(stmt->value, "a");
}

Expr* mock_parse_expr_no_value(TokenBuffer* tokens, size_t* pos,
							   Error* out_error, int num_calls)
{
	*pos += 1;
	return NULL;
}

void test_parse_return_stmt_without_value(void)
{
	TokenBuffer tokens;
	size_t pos = 0;
	Error error = {-1};

	init_token_buffer(&tokens);
	// add tokens
	add_token(&tokens, create_token(TOKEN_RETURN, "return", 6, 1));
	add_token(&tokens, create_token(TOKEN_SEMICOLON, ";", 1, 2));

	parse_expr_StubWithCallback(mock_parse_expr_no_value);

	ReturnStmt* stmt = parse_return_stmt(&tokens, &pos, &error);

	TEST_ASSERT_NOT_NULL(stmt);
	TEST_ASSERT_NULL(stmt->value);
}

void test_parse_return_stmt_missing_semicolon(void)
{
	TokenBuffer tokens;
	size_t pos = 0;
	Error error = {-1};

	init_token_buffer(&tokens);
	// add tokens
	add_token(&tokens, create_token(TOKEN_RETURN, "return", 6, 1));
	add_token(&tokens, create_token(TOKEN_IDENTIFIER, "a", 1, 2));
	// missing semicolon

	parse_expr_StubWithCallback(mock_parse_expr);

	ReturnStmt* stmt = parse_return_stmt(&tokens, &pos, &error);

	TEST_ASSERT_NULL(stmt);
	TEST_ASSERT_EQUAL(SYNTAX_ERROR_UNEXPECTED_TOKEN, error.type);
	TEST_ASSERT_EQUAL_STRING("Expected ';' after return statement",
							 error.message);
}
