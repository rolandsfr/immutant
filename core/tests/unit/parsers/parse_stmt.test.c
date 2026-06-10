// Integration tests for parse_dclr — links libparser.a (see project.yml).

#include "unity.h"

#include "ast_stmt.h"
#include "error_codes.h"
#include "parser_test_support.h"
#include "test_assert_stmt.h"
#include "test_expr.h"

void test_parse_dclr_if_else(void)
{
	TokenBuffer tokens;
	Error error = {-1};
	Stmt* res = init_test_parse_stmt(&tokens, 13,
									 (SampleToken[]){
										 {TOKEN_IF, "if", 2},
										 {TOKEN_LEFT_PAREN, "(", 1},
										 {TOKEN_TRUE, "true", 4},
										 {TOKEN_RIGHT_PAREN, ")", 1},
										 {TOKEN_LEFT_BRACE, "{", 1},
										 {TOKEN_NUMBER, "42", 2},
										 {TOKEN_SEMICOLON, ";", 1},
										 {TOKEN_RIGHT_BRACE, "}", 1},
										 {TOKEN_ELSE, "else", 4},
										 {TOKEN_LEFT_BRACE, "{", 1},
										 {TOKEN_NUMBER, "0", 1},
										 {TOKEN_SEMICOLON, ";", 1},
										 {TOKEN_RIGHT_BRACE, "}", 1},
									 },
									 &error, parse_dclr);

	TEST_ASSERT_NOT_NULL(res);
	TEST_ASSERT_EQUAL_INT(STMT_IF, res->type);
	IfStmt* if_stmt = (IfStmt*)res;
	TEST_ASSERT_LITERAL_BOOL_EXPR(if_stmt->condition, 1);

	BlockStmt* then_block = (BlockStmt*)if_stmt->then_branch;
	TEST_ASSERT_LITERAL_NUMBER_EXPR(
		((ExprStmt*)then_block->statements[0])->expression, "42");

	BlockStmt* else_block = (BlockStmt*)if_stmt->else_branch;
	TEST_ASSERT_LITERAL_NUMBER_EXPR(
		((ExprStmt*)else_block->statements[0])->expression, "0");

	free_token_buffer(&tokens);
}

void test_parse_dclr_if_without_else(void)
{
	TokenBuffer tokens;
	Error error = {-1};
	Stmt* res = init_test_parse_stmt(&tokens, 8,
									 (SampleToken[]){
										 {TOKEN_IF, "if", 2},
										 {TOKEN_LEFT_PAREN, "(", 1},
										 {TOKEN_TRUE, "true", 4},
										 {TOKEN_RIGHT_PAREN, ")", 1},
										 {TOKEN_LEFT_BRACE, "{", 1},
										 {TOKEN_NUMBER, "1", 1},
										 {TOKEN_SEMICOLON, ";", 1},
										 {TOKEN_RIGHT_BRACE, "}", 1},
									 },
									 &error, parse_dclr);

	TEST_ASSERT_NOT_NULL(res);
	IfStmt* if_stmt = (IfStmt*)res;
	TEST_ASSERT_NULL(if_stmt->else_branch);

	free_token_buffer(&tokens);
}

void test_parse_dclr_if_missing_left_paren(void)
{
	TokenBuffer tokens;
	Error error = {-1};
	Stmt* res = init_test_parse_stmt(&tokens, 2,
									 (SampleToken[]){
										 {TOKEN_IF, "if", 2},
										 {TOKEN_TRUE, "true", 4},
									 },
									 &error, parse_dclr);

	TEST_ASSERT_NULL(res);
	TEST_ASSERT_EQUAL(SYNTAX_ERROR_UNEXPECTED_TOKEN, error.type);

	free_token_buffer(&tokens);
}

void test_parse_dclr_if_missing_right_paren(void)
{
	TokenBuffer tokens;
	Error error = {-1};
	Stmt* res = init_test_parse_stmt(&tokens, 3,
									 (SampleToken[]){
										 {TOKEN_IF, "if", 2},
										 {TOKEN_LEFT_PAREN, "(", 1},
										 {TOKEN_TRUE, "true", 4},
									 },
									 &error, parse_dclr);

	TEST_ASSERT_NULL(res);
	TEST_ASSERT_EQUAL(SYNTAX_ERROR_UNEXPECTED_TOKEN, error.type);

	free_token_buffer(&tokens);
}

void test_parse_dclr_if_condition_parse_failure(void)
{
	TokenBuffer tokens;
	Error error = {-1};
	Stmt* res = init_test_parse_stmt(&tokens, 2,
									 (SampleToken[]){
										 {TOKEN_IF, "if", 2},
										 {TOKEN_LEFT_PAREN, "(", 1},
									 },
									 &error, parse_dclr);

	TEST_ASSERT_NULL(res);

	free_token_buffer(&tokens);
}

void test_parse_dclr_var_decl_with_initializer(void)
{
	TokenBuffer tokens;
	Error error = {-1};
	Stmt* res = init_test_parse_stmt(&tokens, 5,
									 (SampleToken[]){
										 {TOKEN_MUTANT, "mut", 3},
										 {TOKEN_IDENTIFIER, "x", 1},
										 {TOKEN_EQUAL, "=", 1},
										 {TOKEN_NUMBER, "42", 2},
										 {TOKEN_SEMICOLON, ";", 1},
									 },
									 &error, parse_dclr);

	TEST_ASSERT_NOT_NULL(res);
	VarDeclStmt* stmt = (VarDeclStmt*)res;
	TEST_ASSERT_VAR_STMT_INIT(stmt, MUTABLE, "x", "42");

	free_token_buffer(&tokens);
}

void test_parse_dclr_var_decl_missing_name(void)
{
	TokenBuffer tokens;
	Error error = {-1};
	Stmt* res = init_test_parse_stmt(&tokens, 4,
									 (SampleToken[]){
										 {TOKEN_MUTANT, "mut", 3},
										 {TOKEN_EQUAL, "=", 1},
										 {TOKEN_NUMBER, "42", 2},
										 {TOKEN_SEMICOLON, ";", 1},
									 },
									 &error, parse_dclr);

	TEST_ASSERT_NULL(res);
	TEST_ASSERT_EQUAL_STRING("Expected variable name", error.message);

	free_token_buffer(&tokens);
}

void test_parse_dclr_var_decl_missing_semicolon(void)
{
	TokenBuffer tokens;
	Error error = {-1};
	Stmt* res = init_test_parse_stmt(&tokens, 4,
									 (SampleToken[]){
										 {TOKEN_MUTANT, "mut", 3},
										 {TOKEN_IDENTIFIER, "x", 1},
										 {TOKEN_EQUAL, "=", 1},
										 {TOKEN_NUMBER, "42", 2},
									 },
									 &error, parse_dclr);

	TEST_ASSERT_NULL(res);
	TEST_ASSERT_EQUAL_STRING("Expected ';' after variable declaration.",
							 error.message);

	free_token_buffer(&tokens);
}

void test_parse_dclr_var_decl_without_initializer(void)
{
	TokenBuffer tokens;
	Error error = {-1};
	Stmt* res = init_test_parse_stmt(&tokens, 3,
									 (SampleToken[]){
										 {TOKEN_MUTANT, "mut", 3},
										 {TOKEN_IDENTIFIER, "y", 1},
										 {TOKEN_SEMICOLON, ";", 1},
									 },
									 &error, parse_dclr);

	TEST_ASSERT_NOT_NULL(res);
	TEST_ASSERT_VAR_STMT((VarDeclStmt*)res, MUTABLE, "y");

	free_token_buffer(&tokens);
}

void test_parse_dclr_immut_without_initializer(void)
{
	TokenBuffer tokens;
	Error error = {-1};
	Stmt* res = init_test_parse_stmt(&tokens, 3,
									 (SampleToken[]){
										 {TOKEN_IMMUTANT, "imut", 4},
										 {TOKEN_IDENTIFIER, "z", 1},
										 {TOKEN_SEMICOLON, ";", 1},
									 },
									 &error, parse_dclr);

	TEST_ASSERT_NULL(res);
	TEST_ASSERT_EQUAL_STRING("Immutable variables must be initialized.",
							 error.message);

	free_token_buffer(&tokens);
}

void test_parse_dclr_return_with_value(void)
{
	TokenBuffer tokens;
	Error error = {-1};
	Stmt* res = init_test_parse_stmt(&tokens, 3,
									 (SampleToken[]){
										 {TOKEN_RETURN, "return", 6},
										 {TOKEN_IDENTIFIER, "a", 1},
										 {TOKEN_SEMICOLON, ";", 1},
									 },
									 &error, parse_dclr);

	TEST_ASSERT_NOT_NULL(res);
	TEST_ASSERT_EQUAL_INT(STMT_RETURN, res->type);
	TEST_ASSERT_VAR_EXPR(((ReturnStmt*)res)->value, "a");

	free_token_buffer(&tokens);
}

void test_parse_dclr_return_without_value(void)
{
	TokenBuffer tokens;
	Error error = {-1};
	Stmt* res = init_test_parse_stmt(&tokens, 2,
									 (SampleToken[]){
										 {TOKEN_RETURN, "return", 6},
										 {TOKEN_SEMICOLON, ";", 1},
									 },
									 &error, parse_dclr);

	TEST_ASSERT_NOT_NULL(res);
	TEST_ASSERT_EQUAL_INT(STMT_RETURN, res->type);
	TEST_ASSERT_NULL(((ReturnStmt*)res)->value);

	free_token_buffer(&tokens);
}

void test_parse_dclr_return_missing_semicolon(void)
{
	TokenBuffer tokens;
	Error error = {-1};
	Stmt* res = init_test_parse_stmt(&tokens, 2,
									 (SampleToken[]){
										 {TOKEN_RETURN, "return", 6},
										 {TOKEN_IDENTIFIER, "a", 1},
									 },
									 &error, parse_dclr);

	TEST_ASSERT_NULL(res);
	TEST_ASSERT_EQUAL(SYNTAX_ERROR_UNEXPECTED_TOKEN, error.type);
	TEST_ASSERT_EQUAL_STRING("Expected ';' after return statement",
							 error.message);

	free_token_buffer(&tokens);
}

void test_parse_dclr_expr_stmt_with_semicolon(void)
{
	TokenBuffer tokens;
	Error error = {-1};
	Stmt* res = init_test_parse_stmt(&tokens, 2,
									 (SampleToken[]){
										 {TOKEN_NUMBER, "10", 2},
										 {TOKEN_SEMICOLON, ";", 1},
									 },
									 &error, parse_dclr);

	TEST_ASSERT_NOT_NULL(res);
	TEST_ASSERT_EQUAL_INT(STMT_EXPR, res->type);
	TEST_ASSERT_LITERAL_NUMBER_EXPR(((ExprStmt*)res)->expression, "10");

	free_token_buffer(&tokens);
}

void test_parse_dclr_expr_stmt_missing_semicolon(void)
{
	TokenBuffer tokens;
	Error error = {-1};
	Stmt* res = init_test_parse_stmt(&tokens, 3,
									 (SampleToken[]){
										 {TOKEN_NUMBER, "5", 1},
										 {TOKEN_PLUS, "+", 1},
										 {TOKEN_NUMBER, "3", 1},
									 },
									 &error, parse_dclr);

	TEST_ASSERT_NULL(res);
	TEST_ASSERT_EQUAL(SYNTAX_ERROR_UNEXPECTED_TOKEN, error.type);
	TEST_ASSERT_EQUAL_STRING("Expected ';' after expression statement.",
							 error.message);

	free_token_buffer(&tokens);
}

void test_parse_dclr_block_stmt(void)
{
	TokenBuffer tokens;
	Error error = {-1};
	Stmt* res = init_test_parse_stmt(&tokens, 9,
									 (SampleToken[]){
										 {TOKEN_LEFT_BRACE, "{", 1},
										 {TOKEN_MUTANT, "mut", 3},
										 {TOKEN_IDENTIFIER, "x", 1},
										 {TOKEN_EQUAL, "=", 1},
										 {TOKEN_NUMBER, "10", 2},
										 {TOKEN_SEMICOLON, ";", 1},
										 {TOKEN_NUMBER, "21", 2},
										 {TOKEN_SEMICOLON, ";", 1},
										 {TOKEN_RIGHT_BRACE, "}", 1},
									 },
									 &error, parse_dclr);

	TEST_ASSERT_NOT_NULL(res);
	TEST_ASSERT_EQUAL_INT(STMT_BLOCK, res->type);
	BlockStmt* block = (BlockStmt*)res;
	TEST_ASSERT_EQUAL_size_t(2, block->count);
	TEST_ASSERT_EQUAL_INT(STMT_VAR_DECL, block->statements[0]->type);

	free_token_buffer(&tokens);
}

void test_parse_dclr_block_missing_closing_brace(void)
{
	TokenBuffer tokens;
	Error error = {-1};
	Stmt* res = init_test_parse_stmt(&tokens, 5,
									 (SampleToken[]){
										 {TOKEN_LEFT_BRACE, "{", 1},
										 {TOKEN_NUMBER, "10", 2},
										 {TOKEN_SEMICOLON, ";", 1},
										 {TOKEN_NUMBER, "21", 2},
										 {TOKEN_SEMICOLON, ";", 1},
									 },
									 &error, parse_dclr);

	TEST_ASSERT_NULL(res);
	TEST_ASSERT_EQUAL_INT(SYNTAX_ERROR_UNEXPECTED_TOKEN, error.type);
	TEST_ASSERT_EQUAL_STRING("Expected '}' after block statement",
							 error.message);

	free_token_buffer(&tokens);
}

void test_parse_dclr_fun_decl(void)
{
	TokenBuffer tokens;
	Error error = {-1};
	Stmt* res = init_test_parse_stmt(&tokens, 9,
									 (SampleToken[]){
										 {TOKEN_FN, "fn", 2},
										 {TOKEN_IDENTIFIER, "fnName", 6},
										 {TOKEN_LEFT_PAREN, "(", 1},
										 {TOKEN_IDENTIFIER, "param1", 6},
										 {TOKEN_COMMA, ",", 1},
										 {TOKEN_IDENTIFIER, "param2", 6},
										 {TOKEN_RIGHT_PAREN, ")", 1},
										 {TOKEN_LEFT_BRACE, "{", 1},
										 {TOKEN_RIGHT_BRACE, "}", 1},
									 },
									 &error, parse_dclr);

	TEST_ASSERT_NOT_NULL(res);
	FunDeclStmt* stmt = (FunDeclStmt*)res;
	TEST_ASSERT_EQUAL_STRING("fnName", stmt->name);
	TEST_ASSERT_EQUAL_INT(2, stmt->params->len);
	TEST_ASSERT_EQUAL_STRING("param1", stmt->params->data[0]);
	TEST_ASSERT_EQUAL_STRING("param2", stmt->params->data[1]);
	TEST_ASSERT_EQUAL_INT(PURE, stmt->purity);

	free_token_buffer(&tokens);
}

void test_parse_dclr_fun_decl_no_name(void)
{
	TokenBuffer tokens;
	Error error = {-1};
	Stmt* res = init_test_parse_stmt(&tokens, 7,
									 (SampleToken[]){
										 {TOKEN_FN, "fn", 2},
										 {TOKEN_LEFT_PAREN, "(", 1},
										 {TOKEN_IDENTIFIER, "param1", 6},
										 {TOKEN_COMMA, ",", 1},
										 {TOKEN_IDENTIFIER, "param2", 6},
										 {TOKEN_RIGHT_PAREN, ")", 1},
										 {TOKEN_LEFT_BRACE, "{", 1},
										 {TOKEN_RIGHT_BRACE, "}", 1},
									 },
									 &error, parse_dclr);

	TEST_ASSERT_NULL(res);
	TEST_ASSERT_EQUAL_INT(SYNTAX_ERROR_UNEXPECTED_TOKEN, error.type);
	TEST_ASSERT_EQUAL_STRING("Expected function name", error.message);

	free_token_buffer(&tokens);
}

void test_parse_dclr_fun_decl_no_params_closing_paren(void)
{
	TokenBuffer tokens;
	Error error = {-1};
	Stmt* res = init_test_parse_stmt(&tokens, 6,
									 (SampleToken[]){
										 {TOKEN_FN, "fn", 2},
										 {TOKEN_IDENTIFIER, "fnName", 6},
										 {TOKEN_LEFT_PAREN, "(", 1},
										 {TOKEN_IDENTIFIER, "param1", 6},
										 {TOKEN_COMMA, ",", 1},
										 {TOKEN_IDENTIFIER, "param2", 6},
										 {TOKEN_LEFT_BRACE, "{", 1},
										 {TOKEN_RIGHT_BRACE, "}", 1},
									 },
									 &error, parse_dclr);

	TEST_ASSERT_NULL(res);
	TEST_ASSERT_EQUAL_STRING("Expected ')' after function parameters",
							 error.message);

	free_token_buffer(&tokens);
}

void test_parse_dclr_fun_decl_no_body_closing_brace(void)
{
	TokenBuffer tokens;
	Error error = {-1};
	Stmt* res = init_test_parse_stmt(&tokens, 8,
									 (SampleToken[]){
										 {TOKEN_FN, "fn", 2},
										 {TOKEN_IDENTIFIER, "fnName", 6},
										 {TOKEN_LEFT_PAREN, "(", 1},
										 {TOKEN_IDENTIFIER, "param1", 6},
										 {TOKEN_COMMA, ",", 1},
										 {TOKEN_IDENTIFIER, "param2", 6},
										 {TOKEN_RIGHT_PAREN, ")", 1},
										 {TOKEN_LEFT_BRACE, "{", 1},
									 },
									 &error, parse_dclr);

	TEST_ASSERT_NULL(res);
	TEST_ASSERT_EQUAL_STRING("Expected '}' after block statement",
							 error.message);

	free_token_buffer(&tokens);
}

void test_parse_dclr_fun_decl_no_params(void)
{
	TokenBuffer tokens;
	Error error = {-1};
	Stmt* res = init_test_parse_stmt(&tokens, 6,
									 (SampleToken[]){
										 {TOKEN_FN, "fn", 2},
										 {TOKEN_IDENTIFIER, "fnName", 6},
										 {TOKEN_LEFT_PAREN, "(", 1},
										 {TOKEN_RIGHT_PAREN, ")", 1},
										 {TOKEN_LEFT_BRACE, "{", 1},
										 {TOKEN_RIGHT_BRACE, "}", 1},
									 },
									 &error, parse_dclr);

	TEST_ASSERT_NOT_NULL(res);
	FunDeclStmt* stmt = (FunDeclStmt*)res;
	TEST_ASSERT_EQUAL_INT(0, stmt->params->len);

	free_token_buffer(&tokens);
}

void test_parse_dclr_fun_decl_with_body(void)
{
	TokenBuffer tokens;
	Error error = {-1};
	Stmt* res = init_test_parse_stmt(&tokens, 9,
									 (SampleToken[]){
										 {TOKEN_IMPURE, "impure", 6},
										 {TOKEN_FN, "fn", 2},
										 {TOKEN_IDENTIFIER, "fnName", 6},
										 {TOKEN_LEFT_PAREN, "(", 1},
										 {TOKEN_RIGHT_PAREN, ")", 1},
										 {TOKEN_LEFT_BRACE, "{", 1},
										 {TOKEN_NUMBER, "42", 2},
										 {TOKEN_SEMICOLON, ";", 1},
										 {TOKEN_RIGHT_BRACE, "}", 1},
									 },
									 &error, parse_dclr);

	TEST_ASSERT_NOT_NULL(res);
	FunDeclStmt* stmt = (FunDeclStmt*)res;
	TEST_ASSERT_EQUAL_STRING("fnName", stmt->name);
	TEST_ASSERT_EQUAL_INT(IMPURE, stmt->purity);
	TEST_ASSERT_EQUAL_INT(1, stmt->body->count);

	free_token_buffer(&tokens);
}
