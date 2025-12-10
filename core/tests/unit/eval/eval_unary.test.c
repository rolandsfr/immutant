#include "eval_unary.h"

#include "unity.h"

#include "assert_value.h"
#include "ast_expr.h"
#include "eval.h"
#include "eval_binary.h"
#include "is_equal.h"
#include "make_runtime_err.h"
#include "make_values.h"
#include "require_t.h"
#include "runtime_err.h"
#include "value_t.h"

void test_eval_unary_should_not_work_with_strings(void)
{
	StringExpr str_expr = {.base = {.type = EXPR_LITERAL_STRING},
						   .value = "omg"};
	UnaryExpr unary_expr = {
		.base = {.type = EXPR_UNARY},
		.operator = TOKEN_BANG, // Using '!' operator for demonstration
		.operand = (Expr*)&str_expr};

	RuntimeError err = {.type = RUNTIME_NO_ERROR, .message = NULL};

	Value result = eval_unary(&unary_expr, &err);

	TEST_ASSERT_RUNTIME_ERROR(&err, RUNTIME_EXPECTED_DIFFERENT_TYPE);
}

void test_eval_unary_should_negate_number(void)
{
	NumberExpr num_expr = {.base = {.type = EXPR_LITERAL_NUMBER},
						   .value = "10"};

	UnaryExpr unary_expr = {.base = {.type = EXPR_UNARY},
							.operator = TOKEN_MINUS,
							.operand = (Expr*)&num_expr};

	RuntimeError err = {.type = RUNTIME_NO_ERROR, .message = NULL};

	Value result = eval_unary(&unary_expr, &err);

	TEST_ASSERT_NUMBER_VALUE(result, -10.0);
}

void test_eval_unary_should_negate_boolean(void)
{
	BooleanExpr bool_expr = {.base = {.type = EXPR_LITERAL_BOOL}, .value = 1};

	UnaryExpr unary_expr = {.base = {.type = EXPR_UNARY},
							.operator = TOKEN_BANG,
							.operand = (Expr*)&bool_expr};

	RuntimeError err = {.type = RUNTIME_NO_ERROR, .message = NULL};

	Value result = eval_unary(&unary_expr, &err);

	TEST_ASSERT_BOOL_VALUE(result, 0);
}

void test_eval_unary_should_eval_binary_inside_unary(void)
{
	NumberExpr num_left = {.base = {.type = EXPR_LITERAL_NUMBER}, .value = "5"};

	NumberExpr num_right = {.base = {.type = EXPR_LITERAL_NUMBER},
							.value = "3"};

	BinaryExpr binary_expr = {.base = {.type = EXPR_BINARY},
							  .operator = TOKEN_PLUS,
							  .left = (Expr*)&num_left,
							  .right = (Expr*)&num_right};

	UnaryExpr unary_expr = {.base = {.type = EXPR_UNARY},
							.operator = TOKEN_MINUS,
							.operand = (Expr*)&binary_expr};

	RuntimeError err = {.type = RUNTIME_NO_ERROR, .message = NULL};

	Value result = eval_unary(&unary_expr, &err);

	TEST_ASSERT_NUMBER_VALUE(result, -8.0);
}
