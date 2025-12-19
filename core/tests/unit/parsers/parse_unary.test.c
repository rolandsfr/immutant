
#include "parse_unary.h"

#include "unity.h"

#include "ast_cnstrct.h"
#include "ast_expr.h"
#include "ast_make_expr.h"
#include "error.h"
#include "error_codes.h"
#include "error_report.h"
#include "is_equal.h"
#include "lexer.h" // TODO: remove after decoupled
#include "parser_helpers.h"
#include "resolve.h" // TODO: remove after decoupled
#include "test_expr.h"

#include "Mockparse_call.h"

Expr* mock_parse_literal_number(TokenBuffer* tokens, size_t* pos,
								Error* out_error, int num_calls)
{
	Token token = consume_token(tokens, pos);
	return (Expr*)make_number_expr(token.lexeme);
}

Expr* mock_parse_with_error(TokenBuffer* tokens, size_t* pos, Error* out_error,
							int num_calls)
{
	out_error->type = SYNTAX_ERROR_UNEXPECTED_TOKEN;
	out_error->line = 1;
	snprintf(out_error->message, sizeof(out_error->message),
			 "Unexpected token");

	return NULL;
}

void test_parse_unary(void)
{
	TokenBuffer tokens;
	Error error;
	size_t pos = 0;

	parse_call_StubWithCallback(mock_parse_literal_number);

	Expr* res = init_test_parse(&tokens, 2,
								(SampleToken[]){
									{TOKEN_MINUS, "-", 1},
									{TOKEN_NUMBER, "2", 1},
								},
								&error, parse_unary);

	TEST_ASSERT_NOT_NULL(res);
	TEST_ASSERT_EQUAL_INT(EXPR_UNARY, res->type);
	TEST_ASSERT_EQUAL_STRING(EXPR_LITERAL_NUMBER,
							 ((UnaryExpr*)res)->operand->type);

	TEST_ASSERT_EQUAL_INT(TOKEN_MINUS, ((UnaryExpr*)res)->operator);

	free_token_buffer(&tokens);
}

void test_parse_unary_should_allow_double_unary(void)
{
	TokenBuffer tokens;
	Error error;

	parse_call_StubWithCallback(mock_parse_literal_number);

	Expr* res = init_test_parse(&tokens, 3,
								(SampleToken[]){
									{TOKEN_MINUS, "-", 1},
									{TOKEN_MINUS, "-", 1},
									{TOKEN_NUMBER, "2", 1},
								},
								&error, parse_unary);

	TEST_ASSERT_NOT_NULL(res);
	TEST_ASSERT_EQUAL_INT(EXPR_UNARY, res->type);
	UnaryExpr* first_unary = (UnaryExpr*)res;
	TEST_ASSERT_EQUAL_INT(TOKEN_MINUS, first_unary->operator);

	TEST_ASSERT_EQUAL_INT(EXPR_UNARY, first_unary->operand->type);
	UnaryExpr* second_unary = (UnaryExpr*)first_unary->operand;
	TEST_ASSERT_EQUAL_INT(TOKEN_MINUS, second_unary->operator);

	TEST_ASSERT_EQUAL_INT(EXPR_LITERAL_NUMBER, second_unary->operand->type);

	free_token_buffer(&tokens);
}

void test_parse_unary_should_return_if_parse_error(void)
{
	TokenBuffer tokens;
	Error error;

	parse_call_StubWithCallback(mock_parse_with_error);

	Expr* res = init_test_parse(&tokens, 2,
								(SampleToken[]){
									{TOKEN_MINUS, "-", 1},
									{TOKEN_LEFT_PAREN, "(", 1},
								},
								&error, parse_unary);

	TEST_ASSERT_NULL(res);
	TEST_ASSERT_EQUAL_INT(SYNTAX_ERROR_UNEXPECTED_TOKEN, error.type);

	free_token_buffer(&tokens);
}
