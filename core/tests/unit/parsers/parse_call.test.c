#include "parse_call.h"

#include "unity.h"

#include "ast_cnstrct.h"
#include "ast_expr.h"
#include "ast_make_expr.h"
#include "lexer.h"
#include "parser_helpers.h"
#include "resolve.h"

#include "Mockparse_expr.h"
#include "Mockparse_primary.h"

Expr* mock_parse_callee(TokenBuffer* tokens, size_t* pos, Error* out_error,
						int num_calls)
{
	if (num_calls == 0) {
		while (peek_token(tokens, *pos) != TOKEN_LEFT_PAREN &&
			   !is_at_end(tokens, *pos)) {
			(*pos)++;
		}
		return (Expr*)make_variable_expr("funcName");
	} else {
		return NULL;
	}
}

Expr* mock_parse_argument(TokenBuffer* tokens, size_t* pos, Error* out_error,
						  int num_calls)
{
	if (num_calls == 0) {
		Expr* arg1 = (Expr*)make_variable_expr("arg1");
		(*pos)++; // consume arg1
		return arg1;
	} else if (num_calls == 1) {
		Expr* arg2 = (Expr*)make_variable_expr("arg2");
		(*pos)++; // consume arg2
		return arg2;
	} else {
		return NULL;
	}
}

void test_parse_call_parses_function_call_with_arguments()
{

	TokenBuffer tokens;
	init_token_buffer(&tokens);
	size_t pos = 0;

	// tokens for: funcName(arg1, arg2)
	add_token(&tokens, create_token(TOKEN_IDENTIFIER, "funcName", 8, 1));
	add_token(&tokens, create_token(TOKEN_LEFT_PAREN, "(", 1, 1));
	add_token(&tokens, create_token(TOKEN_IDENTIFIER, "arg1", 4, 1));
	add_token(&tokens, create_token(TOKEN_COMMA, ",", 1, 1));
	add_token(&tokens, create_token(TOKEN_IDENTIFIER, "arg2", 4, 1));
	add_token(&tokens, create_token(TOKEN_RIGHT_PAREN, ")", 1, 1));

	Error error = {-1};

	// mock parse of callee
	parse_primary_StubWithCallback(mock_parse_callee);

	// mock parse of arguments
	parse_expr_StubWithCallback(mock_parse_argument);

	Expr* result = parse_call(&tokens, &pos, &error);

	TEST_ASSERT_EQUAL_INT(result->type, EXPR_CALL);

	CallExpr* call_expr = (CallExpr*)result;
	TEST_ASSERT_EQUAL_INT(call_expr->arg_count, 2);

	// assert names of callee and arguments
	VariableExpr* callee = (VariableExpr*)call_expr->callee;
	TEST_ASSERT_EQUAL_STRING("funcName", callee->name);

	VariableExpr* arg1 = (VariableExpr*)call_expr->args[0];
	TEST_ASSERT_EQUAL_STRING("arg1", arg1->name);

	VariableExpr* arg2 = (VariableExpr*)call_expr->args[1];
	TEST_ASSERT_EQUAL_STRING("arg2", arg2->name);
}

void test_parse_call_handles_missing_closing_paren()
{
	TokenBuffer tokens;
	init_token_buffer(&tokens);
	size_t pos = 0;

	// tokens for: funcName(arg1, arg2
	add_token(&tokens, create_token(TOKEN_IDENTIFIER, "funcName", 8, 1));
	add_token(&tokens, create_token(TOKEN_LEFT_PAREN, "(", 1, 1));
	add_token(&tokens, create_token(TOKEN_IDENTIFIER, "arg1", 4, 1));
	add_token(&tokens, create_token(TOKEN_COMMA, ",", 1, 1));
	add_token(&tokens, create_token(TOKEN_IDENTIFIER, "arg2", 4, 1));
	// Missing TOKEN_RIGHT_PAREN

	Error error = {-1};

	// mock parse of callee
	parse_primary_StubWithCallback(mock_parse_callee);

	// mock parse of arguments
	parse_expr_StubWithCallback(mock_parse_argument);

	Expr* result = parse_call(&tokens, &pos, &error);

	TEST_ASSERT_NULL(result);
	TEST_ASSERT_EQUAL_INT(SYNTAX_ERROR_UNEXPECTED_TOKEN, error.type);
}

void test_parse_call_handles_no_arguments()
{
	TokenBuffer tokens;
	init_token_buffer(&tokens);
	size_t pos = 0;

	// tokens for: funcName()
	add_token(&tokens, create_token(TOKEN_IDENTIFIER, "funcName", 8, 1));
	add_token(&tokens, create_token(TOKEN_LEFT_PAREN, "(", 1, 1));
	add_token(&tokens, create_token(TOKEN_RIGHT_PAREN, ")", 1, 1));

	Error error = {-1};

	// mock parse of callee
	parse_primary_StubWithCallback(mock_parse_callee);

	Expr* result = parse_call(&tokens, &pos, &error);

	TEST_ASSERT_EQUAL_INT(result->type, EXPR_CALL);

	CallExpr* call_expr = (CallExpr*)result;
	TEST_ASSERT_EQUAL_INT(call_expr->arg_count, 0);

	// assert name of callee
	VariableExpr* callee = (VariableExpr*)call_expr->callee;
	TEST_ASSERT_EQUAL_STRING("funcName", callee->name);
}

void test_parse_throws_error_on_trailing_comma()
{
	TokenBuffer tokens;
	init_token_buffer(&tokens);
	size_t pos = 0;

	// tokens for: funcName(arg1, )
	add_token(&tokens, create_token(TOKEN_IDENTIFIER, "funcName", 8, 1));
	add_token(&tokens, create_token(TOKEN_LEFT_PAREN, "(", 1, 1));
	add_token(&tokens, create_token(TOKEN_IDENTIFIER, "arg1", 4, 1));
	add_token(&tokens, create_token(TOKEN_COMMA, ",", 1, 1));
	// Trailing comma without argument
	add_token(&tokens, create_token(TOKEN_RIGHT_PAREN, ")", 1, 1));

	Error error = {-1};

	// mock parse of callee
	parse_primary_StubWithCallback(mock_parse_callee);

	// mock parse of arguments
	parse_expr_StubWithCallback(mock_parse_argument);

	Expr* result = parse_call(&tokens, &pos, &error);

	TEST_ASSERT_NULL(result);
	TEST_ASSERT_EQUAL_INT(SYNTAX_ERROR_UNEXPECTED_TOKEN, error.type);
}
