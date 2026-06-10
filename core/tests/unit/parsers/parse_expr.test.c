// Integration tests for parse_expr — links libparser.a (see project.yml).

#include "unity.h"

#include "ast_expr.h"
#include "error_codes.h"
#include "parser_test_support.h"
#include "test_expr.h"

void test_parse_expr_equality(void)
{
	TokenBuffer tokens;
	Error error = {-1};
	Expr* res = init_test_parse(&tokens, 3,
								(SampleToken[]){
									{TOKEN_NUMBER, "2", 1},
									{TOKEN_EQUAL_EQUAL, "==", 2},
									{TOKEN_NUMBER, "7", 1},
								},
								&error, parse_expr);

	TEST_ASSERT_NOT_NULL(res);
	BinaryExpr* binary_expr = (BinaryExpr*)res;
	TEST_ASSERT_BINARY_EXPR(binary_expr, TOKEN_EQUAL_EQUAL);
	TEST_ASSERT_LITERAL_NUMBER_EXPR(binary_expr->left, "2");
	TEST_ASSERT_LITERAL_NUMBER_EXPR(binary_expr->right, "7");

	free_expr(res);
	free_token_buffer(&tokens);
}

void test_parse_expr_comparison(void)
{
	TokenBuffer tokens;
	Error error = {-1};
	Expr* res = init_test_parse(&tokens, 3,
								(SampleToken[]){
									{TOKEN_NUMBER, "20", 2},
									{TOKEN_GREATER, ">", 1},
									{TOKEN_NUMBER, "7", 1},
								},
								&error, parse_expr);

	TEST_ASSERT_NOT_NULL(res);
	BinaryExpr* binary_expr = (BinaryExpr*)res;
	TEST_ASSERT_BINARY_EXPR(binary_expr, TOKEN_GREATER);
	TEST_ASSERT_LITERAL_NUMBER_EXPR(binary_expr->left, "20");
	TEST_ASSERT_LITERAL_NUMBER_EXPR(binary_expr->right, "7");

	free_expr(res);
	free_token_buffer(&tokens);
}

void test_parse_expr_comparison_with_precedence(void)
{
	TokenBuffer tokens;
	Error error = {-1};
	Expr* res = init_test_parse(&tokens, 5,
								(SampleToken[]){
									{TOKEN_NUMBER, "2", 1},
									{TOKEN_SLASH, "/", 1},
									{TOKEN_NUMBER, "4", 1},
									{TOKEN_GREATER, ">", 1},
									{TOKEN_NUMBER, "5", 1},
								},
								&error, parse_expr);

	TEST_ASSERT_NOT_NULL(res);
	BinaryExpr* binary_expr = (BinaryExpr*)res;
	TEST_ASSERT_BINARY_EXPR(binary_expr, TOKEN_GREATER);
	TEST_ASSERT_LITERAL_NUMBER_EXPR(binary_expr->right, "5");
	BinaryExpr* left_expr = (BinaryExpr*)binary_expr->left;
	TEST_ASSERT_BINARY_EXPR(left_expr, TOKEN_SLASH);
	TEST_ASSERT_LITERAL_NUMBER_EXPR(left_expr->left, "2");
	TEST_ASSERT_LITERAL_NUMBER_EXPR(left_expr->right, "4");

	free_expr(res);
	free_token_buffer(&tokens);
}

void test_parse_expr_term_precedence(void)
{
	TokenBuffer tokens;
	Error error = {-1};
	Expr* res = init_test_parse(&tokens, 5,
								(SampleToken[]){
									{TOKEN_NUMBER, "2", 1},
									{TOKEN_PLUS, "+", 1},
									{TOKEN_NUMBER, "4", 1},
									{TOKEN_SLASH, "/", 1},
									{TOKEN_NUMBER, "5", 1},
								},
								&error, parse_expr);

	TEST_ASSERT_NOT_NULL(res);
	BinaryExpr* binary_expr = (BinaryExpr*)res;
	TEST_ASSERT_BINARY_EXPR(binary_expr, TOKEN_PLUS);
	TEST_ASSERT_LITERAL_NUMBER_EXPR(binary_expr->left, "2");
	BinaryExpr* right_expr = (BinaryExpr*)binary_expr->right;
	TEST_ASSERT_BINARY_EXPR(right_expr, TOKEN_SLASH);
	TEST_ASSERT_LITERAL_NUMBER_EXPR(right_expr->left, "4");
	TEST_ASSERT_LITERAL_NUMBER_EXPR(right_expr->right, "5");

	free_expr(res);
	free_token_buffer(&tokens);
}

void test_parse_expr_factor_precedence(void)
{
	TokenBuffer tokens;
	Error error = {-1};
	Expr* res = init_test_parse(&tokens, 5,
								(SampleToken[]){
									{TOKEN_NUMBER, "2", 1},
									{TOKEN_SLASH, "/", 1},
									{TOKEN_NUMBER, "3", 1},
									{TOKEN_STAR, "*", 1},
									{TOKEN_NUMBER, "66", 2},
								},
								&error, parse_expr);

	TEST_ASSERT_NOT_NULL(res);
	BinaryExpr* binary_expr = (BinaryExpr*)res;
	BinaryExpr* left_expr = (BinaryExpr*)binary_expr->left;
	TEST_ASSERT_BINARY_EXPR(binary_expr, TOKEN_STAR);
	TEST_ASSERT_BINARY_EXPR(left_expr, TOKEN_SLASH);
	TEST_ASSERT_LITERAL_NUMBER_EXPR(left_expr->left, "2");
	TEST_ASSERT_LITERAL_NUMBER_EXPR(left_expr->right, "3");
	TEST_ASSERT_LITERAL_NUMBER_EXPR(binary_expr->right, "66");

	free_expr(res);
	free_token_buffer(&tokens);
}

void test_parse_expr_missing_operand_error(void)
{
	TokenBuffer tokens;
	Error error = {-1};

	Expr* res = init_test_parse(&tokens, 2,
								(SampleToken[]){
									{TOKEN_NUMBER, "2", 1},
									{TOKEN_EQUAL_EQUAL, "==", 2},
								},
								&error, parse_expr);

	TEST_ASSERT_NULL(res);
	TEST_ASSERT_EQUAL_INT(SYNTAX_ERROR_MISSING_EXPRESSION, error.type);
	TEST_ASSERT_EQUAL_STRING("Expected expression", error.message);

	free_token_buffer(&tokens);
}

void test_parse_expr_unary(void)
{
	TokenBuffer tokens;
	Error error = {-1};
	Expr* res = init_test_parse(&tokens, 2,
								(SampleToken[]){
									{TOKEN_MINUS, "-", 1},
									{TOKEN_NUMBER, "4", 1},
								},
								&error, parse_expr);

	TEST_ASSERT_NOT_NULL(res);
	TEST_ASSERT_UNARY_NUMBER_XPR(res, TOKEN_MINUS, "4");

	free_expr(res);
	free_token_buffer(&tokens);
}

void test_parse_expr_double_unary(void)
{
	TokenBuffer tokens;
	Error error = {-1};
	Expr* res = init_test_parse(&tokens, 3,
								(SampleToken[]){
									{TOKEN_MINUS, "-", 1},
									{TOKEN_MINUS, "-", 1},
									{TOKEN_NUMBER, "2", 1},
								},
								&error, parse_expr);

	TEST_ASSERT_NOT_NULL(res);
	UnaryExpr* first_unary = (UnaryExpr*)res;
	TEST_ASSERT_EQUAL_INT(TOKEN_MINUS, first_unary->operator);
	UnaryExpr* second_unary = (UnaryExpr*)first_unary->operand;
	TEST_ASSERT_EQUAL_INT(TOKEN_MINUS, second_unary->operator);
	TEST_ASSERT_LITERAL_NUMBER_EXPR(second_unary->operand, "2");

	free_expr(res);
	free_token_buffer(&tokens);
}

void test_parse_expr_number_literal(void)
{
	TokenBuffer tokens;
	Error error = {-1};
	Expr* res = init_test_parse(&tokens, 1,
								(SampleToken[]){
									{TOKEN_NUMBER, "2", 1},
								},
								&error, parse_expr);

	TEST_ASSERT_LITERAL_NUMBER_EXPR(res, "2");

	free_expr(res);
	free_token_buffer(&tokens);
}

void test_parse_expr_boolean_literal(void)
{
	TokenBuffer tokens;
	Error error = {-1};
	Expr* res = init_test_parse(&tokens, 1,
								(SampleToken[]){
									{TOKEN_TRUE, "true", 4},
								},
								&error, parse_expr);

	TEST_ASSERT_LITERAL_BOOL_EXPR(res, 1);

	free_expr(res);
	free_token_buffer(&tokens);
}

void test_parse_expr_parenthesized(void)
{
	TokenBuffer tokens;
	Error error = {-1};
	Expr* res = init_test_parse(&tokens, 5,
								(SampleToken[]){
									{TOKEN_LEFT_PAREN, "(", 1},
									{TOKEN_NUMBER, "8", 1},
									{TOKEN_PLUS, "+", 1},
									{TOKEN_NUMBER, "9", 1},
									{TOKEN_RIGHT_PAREN, ")", 1},
								},
								&error, parse_expr);

	TEST_ASSERT_NOT_NULL(res);
	BinaryExpr* binary_expr = (BinaryExpr*)res;
	TEST_ASSERT_BINARY_EXPR(binary_expr, TOKEN_PLUS);
	TEST_ASSERT_LITERAL_NUMBER_EXPR(binary_expr->left, "8");
	TEST_ASSERT_LITERAL_NUMBER_EXPR(binary_expr->right, "9");

	free_expr(res);
	free_token_buffer(&tokens);
}

void test_parse_expr_missing_closing_paren(void)
{
	TokenBuffer tokens;
	Error error = {-1};
	Expr* res = init_test_parse(&tokens, 2,
								(SampleToken[]){
									{TOKEN_LEFT_PAREN, "(", 1},
									{TOKEN_NUMBER, "2", 1},
								},
								&error, parse_expr);

	TEST_ASSERT_NULL(res);
	TEST_ASSERT_EQUAL_INT(SYNTAX_ERROR_UNEXPECTED_TOKEN, error.type);

	free_token_buffer(&tokens);
}

void test_parse_expr_unexpected_token(void)
{
	TokenBuffer tokens;
	Error error = {-1};
	Expr* res = init_test_parse(&tokens, 1,
								(SampleToken[]){
									{TOKEN_UNRECOGNIZED, "@", 1},
								},
								&error, parse_expr);

	TEST_ASSERT_NULL(res);
	TEST_ASSERT_EQUAL_INT(SYNTAX_ERROR_MISSING_EXPRESSION, error.type);

	free_token_buffer(&tokens);
}

void test_parse_expr_identifier(void)
{
	TokenBuffer tokens;
	Error error = {-1};
	Expr* res = init_test_parse(&tokens, 1,
								(SampleToken[]){
									{TOKEN_IDENTIFIER, "myVar", 5},
								},
								&error, parse_expr);

	TEST_ASSERT_VAR_EXPR(res, "myVar");

	free_expr(res);
	free_token_buffer(&tokens);
}

void test_parse_expr_function_call_with_args(void)
{
	TokenBuffer tokens;
	Error error = {-1};
	Expr* res = init_test_parse(&tokens, 6,
								(SampleToken[]){
									{TOKEN_IDENTIFIER, "funcName", 8},
									{TOKEN_LEFT_PAREN, "(", 1},
									{TOKEN_IDENTIFIER, "arg1", 4},
									{TOKEN_COMMA, ",", 1},
									{TOKEN_IDENTIFIER, "arg2", 4},
									{TOKEN_RIGHT_PAREN, ")", 1},
								},
								&error, parse_expr);

	TEST_ASSERT_NOT_NULL(res);
	TEST_ASSERT_EQUAL_INT(EXPR_CALL, res->type);
	CallExpr* call_expr = (CallExpr*)res;
	TEST_ASSERT_EQUAL_INT(2, call_expr->arg_count);
	TEST_ASSERT_EQUAL_STRING("funcName", ((VariableExpr*)call_expr->callee)->name);
	TEST_ASSERT_EQUAL_STRING("arg1", ((VariableExpr*)call_expr->args[0])->name);
	TEST_ASSERT_EQUAL_STRING("arg2", ((VariableExpr*)call_expr->args[1])->name);

	free_expr(res);
	free_token_buffer(&tokens);
}

void test_parse_expr_function_call_no_args(void)
{
	TokenBuffer tokens;
	Error error = {-1};
	Expr* res = init_test_parse(&tokens, 3,
								(SampleToken[]){
									{TOKEN_IDENTIFIER, "funcName", 8},
									{TOKEN_LEFT_PAREN, "(", 1},
									{TOKEN_RIGHT_PAREN, ")", 1},
								},
								&error, parse_expr);

	TEST_ASSERT_NOT_NULL(res);
	TEST_ASSERT_EQUAL_INT(EXPR_CALL, res->type);
	TEST_ASSERT_EQUAL_INT(0, ((CallExpr*)res)->arg_count);

	free_expr(res);
	free_token_buffer(&tokens);
}

void test_parse_expr_function_call_missing_closing_paren(void)
{
	TokenBuffer tokens;
	Error error = {-1};
	Expr* res = init_test_parse(&tokens, 5,
								(SampleToken[]){
									{TOKEN_IDENTIFIER, "funcName", 8},
									{TOKEN_LEFT_PAREN, "(", 1},
									{TOKEN_IDENTIFIER, "arg1", 4},
									{TOKEN_COMMA, ",", 1},
									{TOKEN_IDENTIFIER, "arg2", 4},
								},
								&error, parse_expr);

	TEST_ASSERT_NULL(res);
	TEST_ASSERT_EQUAL_INT(SYNTAX_ERROR_UNEXPECTED_TOKEN, error.type);

	free_token_buffer(&tokens);
}

void test_parse_expr_function_call_trailing_comma(void)
{
	TokenBuffer tokens;
	Error error = {-1};
	Expr* res = init_test_parse(&tokens, 5,
								(SampleToken[]){
									{TOKEN_IDENTIFIER, "funcName", 8},
									{TOKEN_LEFT_PAREN, "(", 1},
									{TOKEN_IDENTIFIER, "arg1", 4},
									{TOKEN_COMMA, ",", 1},
									{TOKEN_RIGHT_PAREN, ")", 1},
								},
								&error, parse_expr);

	TEST_ASSERT_NULL(res);
	TEST_ASSERT_EQUAL_INT(SYNTAX_ERROR_UNEXPECTED_TOKEN, error.type);

	free_token_buffer(&tokens);
}

void test_parse_expr_simple_assignment(void)
{
	TokenBuffer tokens;
	Error error = {-1};
	Expr* res = init_test_parse(&tokens, 3,
								(SampleToken[]){
									{TOKEN_IDENTIFIER, "x", 1},
									{TOKEN_EQUAL, "=", 1},
									{TOKEN_NUMBER, "42", 2},
								},
								&error, parse_expr);

	TEST_ASSERT_NOT_NULL(res);
	TEST_ASSERT_EQUAL(EXPR_ASSIGN, res->type);
	AssignExpr* assign_expr = (AssignExpr*)res;
	TEST_ASSERT_EQUAL_STRING("x", assign_expr->name);
	TEST_ASSERT_LITERAL_NUMBER_EXPR(assign_expr->value, "42");

	free_expr(res);
	free_token_buffer(&tokens);
}

void test_parse_expr_invalid_assignment_target(void)
{
	TokenBuffer tokens;
	Error error = {-1};
	Expr* res = init_test_parse(&tokens, 3,
								(SampleToken[]){
									{TOKEN_NUMBER, "5", 1},
									{TOKEN_EQUAL, "=", 1},
									{TOKEN_NUMBER, "10", 2},
								},
								&error, parse_expr);

	TEST_ASSERT_NULL(res);
	TEST_ASSERT_EQUAL_INT(SYNTAX_ERROR_INVALID_ASSIGNMENT_TARGET, error.type);

	free_token_buffer(&tokens);
}

void test_parse_expr_chained_assignment(void)
{
	TokenBuffer tokens;
	Error error = {-1};
	Expr* res = init_test_parse(&tokens, 5,
								(SampleToken[]){
									{TOKEN_IDENTIFIER, "a", 1},
									{TOKEN_EQUAL, "=", 1},
									{TOKEN_IDENTIFIER, "b", 1},
									{TOKEN_EQUAL, "=", 1},
									{TOKEN_NUMBER, "100", 3},
								},
								&error, parse_expr);

	TEST_ASSERT_NOT_NULL(res);
	AssignExpr* outer = (AssignExpr*)res;
	TEST_ASSERT_EQUAL_STRING("a", outer->name);
	AssignExpr* inner = (AssignExpr*)outer->value;
	TEST_ASSERT_EQUAL_STRING("b", inner->name);
	TEST_ASSERT_LITERAL_NUMBER_EXPR(inner->value, "100");

	free_expr(res);
	free_token_buffer(&tokens);
}

void test_parse_expr_equality_left_associativity(void)
{
	TokenBuffer tokens;
	Error error = {-1};
	Expr* res = init_test_parse(&tokens, 5,
								(SampleToken[]){
									{TOKEN_IDENTIFIER, "a", 1},
									{TOKEN_EQUAL_EQUAL, "==", 2},
									{TOKEN_IDENTIFIER, "b", 1},
									{TOKEN_EQUAL_EQUAL, "==", 2},
									{TOKEN_IDENTIFIER, "c", 1},
								},
								&error, parse_expr);

	TEST_ASSERT_NOT_NULL(res);
	BinaryExpr* outer = (BinaryExpr*)res;
	TEST_ASSERT_EQUAL_INT(TOKEN_EQUAL_EQUAL, outer->operator);
	TEST_ASSERT_VAR_EXPR(outer->right, "c");
	BinaryExpr* inner = (BinaryExpr*)outer->left;
	TEST_ASSERT_EQUAL_INT(TOKEN_EQUAL_EQUAL, inner->operator);
	TEST_ASSERT_VAR_EXPR(inner->left, "a");
	TEST_ASSERT_VAR_EXPR(inner->right, "b");

	free_expr(res);
	free_token_buffer(&tokens);
}

void test_parse_expr_equality_comparison_precedence(void)
{
	TokenBuffer tokens;
	Error error = {-1};
	Expr* res = init_test_parse(&tokens, 5,
								(SampleToken[]){
									{TOKEN_IDENTIFIER, "a", 1},
									{TOKEN_EQUAL_EQUAL, "==", 2},
									{TOKEN_IDENTIFIER, "b", 1},
									{TOKEN_GREATER_EQUAL, ">=", 2},
									{TOKEN_IDENTIFIER, "c", 1},
								},
								&error, parse_expr);

	TEST_ASSERT_NOT_NULL(res);
	TEST_ASSERT_EQUAL(EXPR_BINARY, res->type);
	BinaryExpr* eq = (BinaryExpr*)res;
	TEST_ASSERT_EQUAL_INT(TOKEN_EQUAL_EQUAL, eq->operator);
	TEST_ASSERT_VAR_EXPR(eq->left, "a");

	BinaryExpr* ge = (BinaryExpr*)eq->right;
	TEST_ASSERT_EQUAL_INT(TOKEN_GREATER_EQUAL, ge->operator);
	TEST_ASSERT_VAR_EXPR(ge->left, "b");
	TEST_ASSERT_VAR_EXPR(ge->right, "c");

	free_expr(res);
	free_token_buffer(&tokens);
}
