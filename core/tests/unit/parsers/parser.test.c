// Integration tests for parse() — links libparser.a (see project.yml).

#include "unity.h"

#include "ast_stmt.h"
#include "error.h"
#include "error_codes.h"
#include "parser_test_support.h"
#include "test_assert_stmt.h"
#include "test_expr.h"

void test_parse_should_sync_after_var_decl_error(void)
{
	TokenBuffer tokens;
	init_token_buffer(&tokens);
	ErrorBuffer errors;
	ErrorBuffer_init(&errors);

	add_token(&tokens, create_token(TOKEN_MUTANT, "mutant", 6, 1));
	add_token(&tokens, create_token(TOKEN_IDENTIFIER, "x", 1, 1));
	add_token(&tokens, create_token(TOKEN_EQUAL, "=", 1, 1));
	add_token(&tokens, create_token(TOKEN_NUMBER, "42", 2, 1));
	add_token(&tokens, create_token(TOKEN_IF, "if", 2, 1));
	add_token(&tokens, create_token(TOKEN_LEFT_PAREN, "(", 1, 1));
	add_token(&tokens, create_token(TOKEN_TRUE, "true", 4, 1));
	add_token(&tokens, create_token(TOKEN_RIGHT_PAREN, ")", 1, 1));
	add_token(&tokens, create_token(TOKEN_LEFT_BRACE, "{", 1, 1));
	add_token(&tokens, create_token(TOKEN_NUMBER, "42", 2, 1));
	add_token(&tokens, create_token(TOKEN_SEMICOLON, ";", 1, 1));
	add_token(&tokens, create_token(TOKEN_RIGHT_BRACE, "}", 1, 1));

	Stmts stmts = parse(&tokens, &errors);

	TEST_ASSERT_EQUAL_INT(1, stmts.len);
	TEST_ASSERT_EQUAL_INT(1, errors.len);
	TEST_ASSERT_EQUAL_INT(SYNTAX_ERROR_UNEXPECTED_TOKEN, errors.data[0].type);
	TEST_ASSERT_EQUAL_STRING("Expected ';' after variable declaration.",
							 errors.data[0].message);

	IfStmt* if_stmt = (IfStmt*)stmts.data[0];
	TEST_ASSERT_EQUAL_INT(STMT_IF, if_stmt->base.type);
	TEST_ASSERT_LITERAL_BOOL_EXPR(if_stmt->condition, 1);
	BlockStmt* then_block = (BlockStmt*)if_stmt->then_branch;
	TEST_ASSERT_LITERAL_NUMBER_EXPR(
		((ExprStmt*)then_block->statements[0])->expression, "42");

	free_token_buffer(&tokens);
}
