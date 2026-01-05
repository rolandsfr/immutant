#include "parse_block_stmt.h"

#include "unity.h"

#include "ast_cnstrct.h"
#include "ast_make_expr.h"
#include "ast_make_stmt.h"
#include "error.h"
#include "lexer.h"
#include "parser_helpers.h"
#include "test_expr.h"

#include "Mockparse_dclr.h"
#include "Mockresolve.h"

Stmt* parse_declr_mock(TokenBuffer* tokens, size_t* pos, Error* out_error,
					   int num_calls)
{
	while (!check_token(tokens, *pos, TOKEN_SEMICOLON)) {
		consume_token(tokens, pos);
	}

	// consume the semicolon
	consume_token(tokens, pos);

	if (num_calls == 0) {
		return (Stmt*)make_var_decl_stmt("x", (Expr*)make_number_expr("10", 1),
										 MUTABLE);
	} else {
		return (Stmt*)make_expr_stmt((Expr*)make_binary_expr(
			(Expr*)make_number_expr("10", 1), TOKEN_PLUS,
			(Expr*)make_number_expr("21", 1), 1));
	}
}

static void add_stmts(TokenBuffer* tokens)
{
	add_token(tokens, create_token(TOKEN_MUTANT, "mutant", 6, 1));
	add_token(tokens, create_token(TOKEN_IDENTIFIER, "x", 1, 1));
	add_token(tokens, create_token(TOKEN_EQUAL, "=", 1, 1));
	add_token(tokens, create_token(TOKEN_NUMBER, "10", 2, 1));
	add_token(tokens, create_token(TOKEN_SEMICOLON, ";", 1, 1));
	add_token(tokens, create_token(TOKEN_NUMBER, "10", 2, 1));
	add_token(tokens, create_token(TOKEN_PLUS, "+", 1, 1));
	add_token(tokens, create_token(TOKEN_NUMBER, "21", 2, 1));
	add_token(tokens, create_token(TOKEN_SEMICOLON, ";", 1, 1));
}

void test_parse_block_stmt_calls_parse_dclr_correct_number_of_times(void)
{
	// Arrange
	TokenBuffer tokens;
	init_token_buffer(&tokens);
	size_t pos = 0;
	Error out_error = {-1};

	add_token(&tokens, create_token(TOKEN_LEFT_BRACE, "{", 1, 1));
	add_stmts(&tokens);
	add_token(&tokens, create_token(TOKEN_RIGHT_BRACE, "}", 1, 1));

	parse_dclr_StubWithCallback(parse_declr_mock);

	consume_token(&tokens, &pos);
	BlockStmt* block = parse_block_stmt(&tokens, &pos, &out_error);

	TEST_ASSERT_NOT_NULL(block);
	TEST_ASSERT_EQUAL_size_t(2, block->count);
	Stmt* var_decl_stmt = block->statements[0];
	TEST_ASSERT_EQUAL_INT(STMT_VAR_DECL, var_decl_stmt->type);
}

void test_parse_block_stmt_throws_on_missing_enclosing_right_brace(void)
{
	// Arrange
	TokenBuffer tokens;
	init_token_buffer(&tokens);
	size_t pos = 0;
	Error out_error = {-1};

	add_token(&tokens, create_token(TOKEN_LEFT_BRACE, "{", 1, 1));
	add_stmts(&tokens);

	parse_dclr_StubWithCallback(parse_declr_mock);

	consume_token(&tokens, &pos);
	BlockStmt* block = parse_block_stmt(&tokens, &pos, &out_error);

	TEST_ASSERT_NULL(block);

	TEST_ASSERT_EQUAL_INT(SYNTAX_ERROR_UNEXPECTED_TOKEN, out_error.type);
	TEST_ASSERT_EQUAL_STRING("Expected '}' after block statement",
							 out_error.message);
}
