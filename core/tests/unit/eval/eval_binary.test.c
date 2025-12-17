
#include "eval_binary.h"

#include "unity.h"

#include "assert_value.h"
#include "ast_expr.h"
#include "env.h"
#include "error.h"
#include "eval_asgn.h"
#include "eval_unary.h"
#include "eval_var.h"
#include "is_equal.h"
#include "lexer.h"
#include "make_values.h"
#include "require_t.h"
#include "resolve.h"
#include "value_t.h"

#include "Mockeval.h"
#include "Mockeval_expr.h"

void test_eval_binary_should_concat_strings_on_plus_token(void)
{
	StringExpr str_left = {.base = {.type = EXPR_LITERAL_STRING},
						   .value = "from the "};

	StringExpr str_right = {.base = {.type = EXPR_LITERAL_STRING},
							.value = "outside world"};
	BinaryExpr concat_expr = {.base = {.type = EXPR_BINARY},
							  .operator = TOKEN_PLUS,
							  .left = (Expr*)&str_left,
							  .right = (Expr*)&str_right};

	eval_expr_IgnoreAndReturn(make_string("from the "));
	eval_expr_IgnoreAndReturn(make_string("outside world"));

	Error err = {-1};

	Value result = eval_binary(&concat_expr, &err, NULL);

	TEST_ASSERT_STRING_VALUE(result, "from the outside world");
}

void test_eval_binary_should_error_on_plus_token_with_mismatched_types(void)
{
	NumberExpr num_left = {.base = {.type = EXPR_LITERAL_NUMBER},
						   .value = "42"};

	StringExpr str_right = {.base = {.type = EXPR_LITERAL_STRING},
							.value = "is the answer"};
	BinaryExpr invalid_expr = {.base = {.type = EXPR_BINARY},
							   .operator = TOKEN_PLUS,
							   .left = (Expr*)&num_left,
							   .right = (Expr*)&str_right};

	Error err = {-1};

	eval_expr_IgnoreAndReturn(make_number(42));
	eval_expr_IgnoreAndReturn(make_string("is the answer"));

	Value result = eval_binary(&invalid_expr, &err, NULL);

	TEST_ASSERT_RUNTIME_ERROR(&err, RUNTIME_UNEXPECTED_TYPE);
}

void test_eval_binary_should_add_numbers_on_plus_token(void)
{
	NumberExpr num_left = {.base = {.type = EXPR_LITERAL_NUMBER},
						   .value = "10"};

	NumberExpr num_right = {.base = {.type = EXPR_LITERAL_NUMBER},
							.value = "32.5"};
	BinaryExpr add_expr = {.base = {.type = EXPR_BINARY},
						   .operator = TOKEN_PLUS,
						   .left = (Expr*)&num_left,
						   .right = (Expr*)&num_right};

	Error err = {-1};

	eval_expr_IgnoreAndReturn(make_number(10));
	eval_expr_IgnoreAndReturn(make_number(32.5));

	Value result = eval_binary(&add_expr, &err, NULL);

	TEST_ASSERT_NUMBER_VALUE(result, 42.5);
}

void test_eval_binary_should_subtract_numbers_on_minus_token(void)
{
	NumberExpr num_left = {.base = {.type = EXPR_LITERAL_NUMBER},
						   .value = "-100"};

	NumberExpr num_right = {.base = {.type = EXPR_LITERAL_NUMBER},
							.value = "58.5"};
	BinaryExpr sub_expr = {.base = {.type = EXPR_BINARY},
						   .operator = TOKEN_MINUS,
						   .left = (Expr*)&num_left,
						   .right = (Expr*)&num_right};

	Error err = {-1};

	eval_expr_IgnoreAndReturn(make_number(-100));
	eval_expr_IgnoreAndReturn(make_number(58.5));

	Value result = eval_binary(&sub_expr, &err, NULL);

	TEST_ASSERT_NUMBER_VALUE(result, -158.5);
}

void test_eval_binary_should_multiply_numbers_on_star_token(void)
{
	NumberExpr num_left = {.base = {.type = EXPR_LITERAL_NUMBER}, .value = "6"};

	NumberExpr num_right = {.base = {.type = EXPR_LITERAL_NUMBER},
							.value = "7"};
	BinaryExpr mul_expr = {.base = {.type = EXPR_BINARY},
						   .operator = TOKEN_STAR,
						   .left = (Expr*)&num_left,
						   .right = (Expr*)&num_right};

	Error err = {-1};

	eval_expr_IgnoreAndReturn(make_number(6));
	eval_expr_IgnoreAndReturn(make_number(7));

	Value result = eval_binary(&mul_expr, &err, NULL);

	TEST_ASSERT_NUMBER_VALUE(result, 42);
}

void test_eval_binary_should_divide_numbers_on_slash_token(void)
{
	NumberExpr num_left = {.base = {.type = EXPR_LITERAL_NUMBER},
						   .value = "84"};

	NumberExpr num_right = {.base = {.type = EXPR_LITERAL_NUMBER},
							.value = "2"};
	BinaryExpr div_expr = {.base = {.type = EXPR_BINARY},
						   .operator = TOKEN_SLASH,
						   .left = (Expr*)&num_left,
						   .right = (Expr*)&num_right};

	Error err = {-1};

	eval_expr_IgnoreAndReturn(make_number(84));
	eval_expr_IgnoreAndReturn(make_number(2));

	Value result = eval_binary(&div_expr, &err, NULL);

	TEST_ASSERT_NUMBER_VALUE(result, 42);
}

void test_eval_binary_should_return_floating_point_result_on_division_according_to_IEEE_standard(
	void)
{
	NumberExpr num_left = {.base = {.type = EXPR_LITERAL_NUMBER},
						   .value = "10"};

	NumberExpr num_right = {.base = {.type = EXPR_LITERAL_NUMBER},
							.value = "3"};
	BinaryExpr div_expr = {.base = {.type = EXPR_BINARY},
						   .operator = TOKEN_SLASH,
						   .left = (Expr*)&num_left,
						   .right = (Expr*)&num_right};

	Error err = {-1};

	eval_expr_IgnoreAndReturn(make_number(10));
	eval_expr_IgnoreAndReturn(make_number(3));

	Value result = eval_binary(&div_expr, &err, NULL);

	TEST_ASSERT_NUMBER_VALUE(result, 3.33333333333333);
}

void test_eval_binary_should_error_on_divide_by_zero(void)
{
	NumberExpr num_left = {.base = {.type = EXPR_LITERAL_NUMBER},
						   .value = "42"};

	NumberExpr num_right = {.base = {.type = EXPR_LITERAL_NUMBER},
							.value = "0"};
	BinaryExpr div_expr = {.base = {.type = EXPR_BINARY},
						   .operator = TOKEN_SLASH,
						   .left = (Expr*)&num_left,
						   .right = (Expr*)&num_right};

	Error err = {-1};

	eval_expr_IgnoreAndReturn(make_number(42));
	eval_expr_IgnoreAndReturn(make_number(0));

	Value result = eval_binary(&div_expr, &err, NULL);

	TEST_ASSERT_RUNTIME_ERROR(&err, RUNTIME_DIVIDE_BY_ZERO);
}

void test_eval_binary_should_return_0_on_0_divided_by_number(void)
{
	NumberExpr num_left = {.base = {.type = EXPR_LITERAL_NUMBER}, .value = "0"};

	NumberExpr num_right = {.base = {.type = EXPR_LITERAL_NUMBER},
							.value = "3.14"};
	BinaryExpr div_expr = {.base = {.type = EXPR_BINARY},
						   .operator = TOKEN_SLASH,
						   .left = (Expr*)&num_left,
						   .right = (Expr*)&num_right};

	eval_expr_IgnoreAndReturn(make_number(0));
	eval_expr_IgnoreAndReturn(make_number(3.14));

	Error err = {-1};

	Value result = eval_binary(&div_expr, &err, NULL);

	TEST_ASSERT_NUMBER_VALUE(result, 0);
}

void test_eval_binary_should_compare_numbers_non_stricly(void)
{
	NumberExpr num_left = {.base = {.type = EXPR_LITERAL_NUMBER},
						   .value = "10"};

	NumberExpr num_right = {.base = {.type = EXPR_LITERAL_NUMBER},
							.value = "20"};

	BinaryExpr comp_expr = {.base = {.type = EXPR_BINARY},
							.operator = TOKEN_LESS,
							.left = (Expr*)&num_left,
							.right = (Expr*)&num_right};

	Error err = {-1};

	eval_expr_IgnoreAndReturn(make_number(10));
	eval_expr_IgnoreAndReturn(make_number(20));

	Value result = eval_binary(&comp_expr, &err, NULL);
	TEST_ASSERT_BOOL_VALUE(result, 1);

	comp_expr.operator = TOKEN_GREATER;
	result = eval_binary(&comp_expr, &err, NULL);
	TEST_ASSERT_BOOL_VALUE(result, 0);

	comp_expr.operator = TOKEN_LESS_EQUAL;
	result = eval_binary(&comp_expr, &err, NULL);
	TEST_ASSERT_BOOL_VALUE(result, 1);
}

void test_eval_binary_should_compare_numbers_stricly(void)
{
	NumberExpr num_left = {.base = {.type = EXPR_LITERAL_NUMBER},
						   .value = "10"};

	NumberExpr num_right = {.base = {.type = EXPR_LITERAL_NUMBER},
							.value = "10"};

	BinaryExpr comp_expr = {.base = {.type = EXPR_BINARY},
							.operator = TOKEN_GREATER_EQUAL,
							.left = (Expr*)&num_left,
							.right = (Expr*)&num_right};

	Error err = {-1};

	eval_expr_IgnoreAndReturn(make_number(10));
	eval_expr_IgnoreAndReturn(make_number(10));

	Value result = eval_binary(&comp_expr, &err, NULL);
	TEST_ASSERT_BOOL_VALUE(result, 1);

	comp_expr.operator = TOKEN_LESS_EQUAL;
	result = eval_binary(&comp_expr, &err, NULL);
	TEST_ASSERT_BOOL_VALUE(result, 1);
}

void test_eval_binary_should_immediately_return_false_on_inequal_types_for_equality_operator(
	void)
{
	NumberExpr num_left = {.base = {.type = EXPR_LITERAL_NUMBER},
						   .value = "42"};

	StringExpr str_right = {.base = {.type = EXPR_LITERAL_STRING},
							.value = "42"};

	BinaryExpr eq_expr = {.base = {.type = EXPR_BINARY},
						  .operator = TOKEN_EQUAL_EQUAL,
						  .left = (Expr*)&num_left,
						  .right = (Expr*)&str_right};

	eval_expr_IgnoreAndReturn(make_number(42));
	eval_expr_IgnoreAndReturn(make_string("42"));

	Error err = {-1};

	Value result = eval_binary(&eq_expr, &err, NULL);
	TEST_ASSERT_BOOL_VALUE(result, 0);
}

void test_eval_binary_should_compare_equal_numbers_on_equality_operator(void)
{
	NumberExpr num_left = {.base = {.type = EXPR_LITERAL_NUMBER},
						   .value = "3.14"};

	NumberExpr num_right = {.base = {.type = EXPR_LITERAL_NUMBER},
							.value = "3.14"};

	BinaryExpr eq_expr = {.base = {.type = EXPR_BINARY},
						  .operator = TOKEN_EQUAL_EQUAL,
						  .left = (Expr*)&num_left,
						  .right = (Expr*)&num_right};

	Error err = {-1};

	eval_expr_IgnoreAndReturn(make_number(3.14));
	eval_expr_IgnoreAndReturn(make_number(3.14));

	Value result = eval_binary(&eq_expr, &err, NULL);
	TEST_ASSERT_BOOL_VALUE(result, 1);
}

void test_eval_binary_should_compare_unequal_strings_on_inequality_operator(
	void)
{
	StringExpr str_left = {.base = {.type = EXPR_LITERAL_STRING},
						   .value = "hello"};

	StringExpr str_right = {.base = {.type = EXPR_LITERAL_STRING},
							.value = "world"};

	BinaryExpr eq_expr = {.base = {.type = EXPR_BINARY},
						  .operator = TOKEN_BANG_EQUAL,
						  .left = (Expr*)&str_left,
						  .right = (Expr*)&str_right};

	Error err = {-1};

	eval_expr_IgnoreAndReturn(make_string("hello"));
	eval_expr_IgnoreAndReturn(make_string("world"));

	Value result = eval_binary(&eq_expr, &err, NULL);
	TEST_ASSERT_BOOL_VALUE(result, 1);
}
