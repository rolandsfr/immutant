#include "parse_var_dclr.h"

#include "unity.h"

#include "ast_cnstrct.h"
#include "ast_make_expr.h"
#include "ast_make_stmt.h"
#include "ast_stmt.h"
#include "error.h"
#include "lexer.h"
#include "parser_helpers.h"
#include "resolve.h"
#include "test_assert_stmt.h"

#include "Mockparse_expr.h"

Expr* parse_expr_stub_cb(TokenBuffer* tokens, size_t* pos, Error* out_error,
						 int num_calls)
{
	// Create a dummy expression to return
	Expr* expr = (Expr*)make_number_expr("42", 1);
	*pos += 1; // Advance position to simulate consumption of token
	return expr;
}

void test_parse_var_decl()
{
	TokenBuffer tokens;
	init_token_buffer(&tokens);
	size_t pos = 1; // starting at x, because function expects pos after keyword
	Error error = {-1};

	parse_expr_StubWithCallback(parse_expr_stub_cb);

	add_token(&tokens, create_token(TOKEN_MUTANT, "mut", 6, 1));
	add_token(&tokens, create_token(TOKEN_IDENTIFIER, "x", 1, 1));
	add_token(&tokens, create_token(TOKEN_EQUAL, "=", 1, 1));
	add_token(&tokens, create_token(TOKEN_NUMBER, "42", 2, 1));
	add_token(&tokens, create_token(TOKEN_SEMICOLON, ";", 1, 1));

	VarDeclStmt* stmt = parse_var_decl(&tokens, &pos, MUTABLE, &error);

	TEST_ASSERT_VAR_STMT_INIT(stmt, MUTABLE, "x", "42");
}

void test_parse_var_decl_should_error_if_no_name()
{
	TokenBuffer tokens;
	init_token_buffer(&tokens);
	size_t pos = 1; // starting at x, because function expects pos after keyword
	Error error = {-1};

	add_token(&tokens, create_token(TOKEN_MUTANT, "mut", 6, 1));
	add_token(&tokens, create_token(TOKEN_EQUAL, "=", 1, 1));
	add_token(&tokens, create_token(TOKEN_NUMBER, "42", 2, 1));
	add_token(&tokens, create_token(TOKEN_SEMICOLON, ";", 1, 1));

	VarDeclStmt* stmt = parse_var_decl(&tokens, &pos, IMMUTABLE, &error);

	TEST_ASSERT_NULL(stmt);
	TEST_ASSERT_EQUAL_STRING("Expected variable name", error.message);
}

void test_parse_var_decl_should_error_if_no_semicolon_after_initializer()
{
	TokenBuffer tokens;
	init_token_buffer(&tokens);
	size_t pos = 1; // starting at x, because function expects pos after keyword
	Error error = {-1};

	parse_expr_StubWithCallback(parse_expr_stub_cb);

	add_token(&tokens, create_token(TOKEN_MUTANT, "mut", 6, 1));
	add_token(&tokens, create_token(TOKEN_IDENTIFIER, "x", 1, 1));
	add_token(&tokens, create_token(TOKEN_EQUAL, "=", 1, 1));
	add_token(&tokens, create_token(TOKEN_NUMBER, "42", 2, 1));
	// Missing semicolon token

	VarDeclStmt* stmt = parse_var_decl(&tokens, &pos, MUTABLE, &error);

	TEST_ASSERT_NULL(stmt);
	TEST_ASSERT_EQUAL_STRING("Expected ';' after variable declaration.",
							 error.message);
}

void test_parse_var_decl_without_initializer()
{
	TokenBuffer tokens;
	init_token_buffer(&tokens);
	size_t pos = 1; // starting at x, because function expects pos after keyword
	Error error = {-1};

	add_token(&tokens, create_token(TOKEN_MUTANT, "mut", 6, 1));
	add_token(&tokens, create_token(TOKEN_IDENTIFIER, "y", 1, 1));
	add_token(&tokens, create_token(TOKEN_SEMICOLON, ";", 1, 1));

	VarDeclStmt* stmt = parse_var_decl(&tokens, &pos, MUTABLE, &error);

	TEST_ASSERT_VAR_STMT(stmt, MUTABLE, "y");
}

void test_parse_var_decl_should_throw_if_imut_have_no_initializer()
{
	TokenBuffer tokens;
	init_token_buffer(&tokens);
	size_t pos = 1; // starting at x, because function expects pos after keyword
	Error error = {-1};

	add_token(&tokens, create_token(TOKEN_IMMUTANT, "imut", 9, 1));
	add_token(&tokens, create_token(TOKEN_IDENTIFIER, "z", 1, 1));
	add_token(&tokens, create_token(TOKEN_SEMICOLON, ";", 1, 1));

	VarDeclStmt* stmt = parse_var_decl(&tokens, &pos, IMMUTABLE, &error);

	TEST_ASSERT_NULL(stmt);
	TEST_ASSERT_EQUAL_STRING("Immutable variables must be initialized.",
							 error.message);
}
