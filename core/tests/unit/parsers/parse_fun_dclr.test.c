#include "parse_fun_dclr.h"

#include "unity.h"

#include "ast_cnstrct.h"
#include "ast_make_expr.h"
#include "ast_make_stmt.h"
#include "ast_stmt.h"
#include "error.h"
#include "lexer.h"
#include "parse_block_stmt.h"
#include "parser_helpers.h"
#include "resolve.h"
#include "test_assert_stmt.h"
#include "test_expr.h"

#include "Mockparse_dclr.h"

void test_parse_var_decl()
{
	TokenBuffer tokens;
	init_token_buffer(&tokens);
	size_t pos = 0;
	Error error = {-1};

	add_token(&tokens, create_token(TOKEN_IDENTIFIER, "fnName", 6, 4));
	add_token(&tokens, create_token(TOKEN_LEFT_PAREN, "(", 1, 4));
	add_token(&tokens, create_token(TOKEN_IDENTIFIER, "param1", 6, 4));
	add_token(&tokens, create_token(TOKEN_COMMA, ",", 1, 4));
	add_token(&tokens, create_token(TOKEN_IDENTIFIER, "param2", 6, 4));
	add_token(&tokens, create_token(TOKEN_RIGHT_PAREN, ")", 1, 4));
	add_token(&tokens, create_token(TOKEN_LEFT_BRACE, "{", 1, 4));
	add_token(&tokens, create_token(TOKEN_RIGHT_BRACE, "}", 1, 5));

	FunDeclStmt* stmt = parse_fun_decl(&tokens, &pos, &error);
	TEST_ASSERT_NOT_NULL(stmt);
	TEST_ASSERT_EQUAL_STRING("fnName", stmt->name);
	TEST_ASSERT_EQUAL_INT(2, stmt->params->len);
	TEST_ASSERT_EQUAL_STRING("param1", stmt->params->data[0]);
	TEST_ASSERT_EQUAL_STRING("param2", stmt->params->data[1]);
	TEST_ASSERT_EQUAL_INT(PURE, stmt->purity);
}

void test_parse_fun_decl_should_error_if_no_name()
{

	TokenBuffer tokens;
	init_token_buffer(&tokens);
	size_t pos = 0;
	Error error = {-1};

	add_token(&tokens, create_token(TOKEN_LEFT_PAREN, "(", 1, 4));
	add_token(&tokens, create_token(TOKEN_IDENTIFIER, "param1", 6, 4));
	add_token(&tokens, create_token(TOKEN_COMMA, ",", 1, 4));
	add_token(&tokens, create_token(TOKEN_IDENTIFIER, "param2", 6, 4));
	add_token(&tokens, create_token(TOKEN_RIGHT_PAREN, ")", 1, 4));
	add_token(&tokens, create_token(TOKEN_LEFT_BRACE, "{", 1, 4));
	add_token(&tokens, create_token(TOKEN_RIGHT_BRACE, "}", 1, 5));

	FunDeclStmt* stmt = parse_fun_decl(&tokens, &pos, &error);

	TEST_ASSERT_NULL(stmt);
	TEST_ASSERT_EQUAL_INT(SYNTAX_ERROR_UNEXPECTED_TOKEN, error.type);
	TEST_ASSERT_EQUAL_STRING("Expected function name or purity specifier",
							 error.message);
}

void test_parse_fun_decl_should_error_if_no_params_closing_paren()
{
	TokenBuffer tokens;
	init_token_buffer(&tokens);
	size_t pos = 0;
	Error error = {-1};

	add_token(&tokens, create_token(TOKEN_IDENTIFIER, "fnName", 6, 4));
	add_token(&tokens, create_token(TOKEN_LEFT_PAREN, "(", 1, 4));
	add_token(&tokens, create_token(TOKEN_IDENTIFIER, "param1", 6, 4));
	add_token(&tokens, create_token(TOKEN_COMMA, ",", 1, 4));
	add_token(&tokens, create_token(TOKEN_IDENTIFIER, "param2", 6, 4));
	// Missing RIGHT_PAREN token
	add_token(&tokens, create_token(TOKEN_LEFT_BRACE, "{", 1, 4));
	add_token(&tokens, create_token(TOKEN_RIGHT_BRACE, "}", 1, 5));

	FunDeclStmt* stmt = parse_fun_decl(&tokens, &pos, &error);

	TEST_ASSERT_NULL(stmt);
	TEST_ASSERT_EQUAL_INT(SYNTAX_ERROR_UNEXPECTED_TOKEN, error.type);
	TEST_ASSERT_EQUAL_STRING("Expected ')' after function parameters",
							 error.message);
}

void test_parse_fun_decl_should_error_if_no_body_closing_brace()
{
	TokenBuffer tokens;
	init_token_buffer(&tokens);
	size_t pos = 0;
	Error error = {-1};

	add_token(&tokens, create_token(TOKEN_IDENTIFIER, "fnName", 6, 4));
	add_token(&tokens, create_token(TOKEN_LEFT_PAREN, "(", 1, 4));
	add_token(&tokens, create_token(TOKEN_IDENTIFIER, "param1", 6, 4));
	add_token(&tokens, create_token(TOKEN_COMMA, ",", 1, 4));
	add_token(&tokens, create_token(TOKEN_IDENTIFIER, "param2", 6, 4));
	add_token(&tokens, create_token(TOKEN_RIGHT_PAREN, ")", 1, 4));
	add_token(&tokens, create_token(TOKEN_LEFT_BRACE, "{", 1, 4));
	// Missing RIGHT_BRACE token

	FunDeclStmt* stmt = parse_fun_decl(&tokens, &pos, &error);

	TEST_ASSERT_NULL(stmt);
	TEST_ASSERT_EQUAL_INT(SYNTAX_ERROR_UNEXPECTED_TOKEN, error.type);
	TEST_ASSERT_EQUAL_STRING("Expected '}' after block statement",
							 error.message);
}

void test_parse_fun_decl_with_no_params()
{
	TokenBuffer tokens;
	init_token_buffer(&tokens);
	size_t pos = 0;
	Error error = {-1};

	add_token(&tokens, create_token(TOKEN_IDENTIFIER, "fnName", 6, 4));
	add_token(&tokens, create_token(TOKEN_LEFT_PAREN, "(", 1, 4));
	// No parameters
	add_token(&tokens, create_token(TOKEN_RIGHT_PAREN, ")", 1, 4));
	add_token(&tokens, create_token(TOKEN_LEFT_BRACE, "{", 1, 4));
	add_token(&tokens, create_token(TOKEN_RIGHT_BRACE, "}", 1, 5));

	FunDeclStmt* stmt = parse_fun_decl(&tokens, &pos, &error);

	TEST_ASSERT_NOT_NULL(stmt);
	TEST_ASSERT_EQUAL_STRING("fnName", stmt->name);
	TEST_ASSERT_EQUAL_INT(0, stmt->params->len);
	TEST_ASSERT_EQUAL_INT(PURE, stmt->purity);
}

Stmt* mock_parse_dclr(TokenBuffer* tokens, size_t* pos, Error* out_error,
					  int num_calls)

{

	if (num_calls == 0) {
		while (peek_token(tokens, *pos) != TOKEN_RIGHT_BRACE &&
			   !is_at_end(tokens, *pos)) {
			(*pos)++;
		}

		Stmt* stmt = (Stmt*)make_expr_stmt((Expr*)make_number_expr("42", 2), 2);
		return stmt;
	} else {
		return NULL;
	}
}

void test_parse_fun_decl_with_body_statements()
{
	TokenBuffer tokens;
	init_token_buffer(&tokens);
	size_t pos = 0;
	Error error = {-1};

	Stmt* stmt = (Stmt*)make_expr_stmt((Expr*)make_number_expr("42", 2), 2);

	parse_dclr_StubWithCallback(mock_parse_dclr);

	add_token(&tokens, create_token(TOKEN_IMPURE, "impure", 6, 4));
	add_token(&tokens, create_token(TOKEN_IDENTIFIER, "fnName", 6, 4));
	add_token(&tokens, create_token(TOKEN_LEFT_PAREN, "(", 1, 4));
	add_token(&tokens, create_token(TOKEN_RIGHT_PAREN, ")", 1, 4));
	add_token(&tokens, create_token(TOKEN_LEFT_BRACE, "{", 1, 4));
	add_token(&tokens, create_token(TOKEN_NUMBER, "42", 2, 5));
	add_token(&tokens, create_token(TOKEN_SEMICOLON, ";", 1, 5));
	add_token(&tokens, create_token(TOKEN_RIGHT_BRACE, "}", 1, 6));

	FunDeclStmt* fun_dclr = parse_fun_decl(&tokens, &pos, &error);

	TEST_ASSERT_NOT_NULL(stmt);
	TEST_ASSERT_EQUAL_STRING("fnName", fun_dclr->name);
	TEST_ASSERT_EQUAL_INT(0, fun_dclr->params->len);
	TEST_ASSERT_EQUAL_INT(IMPURE, fun_dclr->purity);
	TEST_ASSERT_EQUAL_INT(1, fun_dclr->body->count);
}
