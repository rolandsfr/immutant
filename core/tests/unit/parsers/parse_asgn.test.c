#include "parse_asgn.h"

#include "unity.h"

#include "ast_cnstrct.h"
#include "ast_make_expr.h"
#include "error.h"
#include "lexer.h"
#include "parser_helpers.h"
#include "resolve.h"
#include "test_expr.h"

#include "Mockparse_eq.h"

Expr* mock_parse_equality_correct_asgn(TokenBuffer* tokens, size_t* pos,
									   Error* out_error, int num_calls)
{
	if (num_calls == 0) {
		*pos = 1;
		return (Expr*)make_variable_expr("x");
	} else if (num_calls == 1) {
		*pos = 3;
		return (Expr*)make_number_expr("42");
	} else {
		return NULL;
	}
}

Expr* mock_parse_equality_invalid_asgn_target(TokenBuffer* tokens, size_t* pos,
											  Error* out_error, int num_calls)
{
	if (num_calls == 0) {
		*pos = 1;
		return (Expr*)make_number_expr("5");
	} else if (num_calls == 1) {
		*pos = 3;
		return (Expr*)make_variable_expr("b");
	} else {
		return NULL;
	}
}

Expr* mock_parse_several_asgn_exprs(TokenBuffer* tokens, size_t* pos,
									Error* out_error, int num_calls)
{
	if (num_calls == 0) {
		*pos = 1;
		return (Expr*)make_variable_expr("a");
	} else if (num_calls == 1) {
		*pos = 3;
		return (Expr*)make_variable_expr("b");
	} else if (num_calls == 2) {
		*pos = 5;
		return (Expr*)make_number_expr("100");
	} else {
		return NULL;
	}
}

void test_parse_asgn_simple_assignment(void)
{
	TokenBuffer tokens;
	init_token_buffer(&tokens);

	add_token(&tokens, create_token(TOKEN_IDENTIFIER, "x", 1, 1));
	add_token(&tokens, create_token(TOKEN_EQUAL, "=", 1, 2));
	add_token(&tokens, create_token(TOKEN_NUMBER, "42", 1, 3));
	add_token(&tokens, create_token(TOKEN_SEMICOLON, ";", 1, 3));

	parse_equality_StubWithCallback(mock_parse_equality_correct_asgn);

	size_t pos = 0;
	Error error = {-1};

	Expr* result = parse_asgn(&tokens, &pos, &error);

	TEST_ASSERT_NOT_NULL(result);
	TEST_ASSERT_EQUAL(EXPR_ASSIGN, result->type);

	AssignExpr* assign_expr = (AssignExpr*)result;
	TEST_ASSERT_EQUAL_STRING("x", assign_expr->name);

	TEST_ASSERT_LITERAL_NUMBER_EXPR(assign_expr->value, "42");
}

void test_parse_asgn_invalid_assignment_target(void)
{
	TokenBuffer tokens;
	init_token_buffer(&tokens);

	add_token(&tokens, create_token(TOKEN_NUMBER, "5", 1, 1));
	add_token(&tokens, create_token(TOKEN_EQUAL, "=", 1, 2));
	add_token(&tokens, create_token(TOKEN_NUMBER, "10", 1, 3));
	add_token(&tokens, create_token(TOKEN_SEMICOLON, ";", 1, 4));

	parse_equality_StubWithCallback(mock_parse_equality_invalid_asgn_target);

	size_t pos = 0;
	Error error = {-1};

	Expr* result = parse_asgn(&tokens, &pos, &error);

	TEST_ASSERT_NULL(result);
	TEST_ASSERT_EQUAL_INT(SYNTAX_ERROR_INVALID_ASSIGNMENT_TARGET, error.type);

	free_token_buffer(&tokens);
}

void test_parse_asgn_should_return_asgn_target_expr_on_multiple_calls(void)
{
	TokenBuffer tokens;
	init_token_buffer(&tokens);

	add_token(&tokens, create_token(TOKEN_IDENTIFIER, "a", 1, 1));
	add_token(&tokens, create_token(TOKEN_EQUAL, "=", 1, 2));
	add_token(&tokens, create_token(TOKEN_IDENTIFIER, "b", 1, 3));
	add_token(&tokens, create_token(TOKEN_EQUAL, "=", 1, 4));
	add_token(&tokens, create_token(TOKEN_NUMBER, "100", 1, 5));
	add_token(&tokens, create_token(TOKEN_SEMICOLON, ";", 1, 6));

	parse_equality_StubWithCallback(mock_parse_several_asgn_exprs);

	size_t pos = 0;
	Error error = {-1};

	Expr* result = parse_asgn(&tokens, &pos, &error);

	TEST_ASSERT_NOT_NULL(result);
	TEST_ASSERT_EQUAL(EXPR_ASSIGN, result->type);

	AssignExpr* outer_assign = (AssignExpr*)result;
	TEST_ASSERT_EQUAL_STRING("a", outer_assign->name);

	TEST_ASSERT_EQUAL(EXPR_ASSIGN, outer_assign->value->type);
	AssignExpr* inner_assign = (AssignExpr*)outer_assign->value;
	TEST_ASSERT_EQUAL_STRING("b", inner_assign->name);

	TEST_ASSERT_LITERAL_NUMBER_EXPR(inner_assign->value, "100");
	TEST_ASSERT_EQUAL_INT(outer_assign->value->type, EXPR_ASSIGN);

	free_token_buffer(&tokens);
}
