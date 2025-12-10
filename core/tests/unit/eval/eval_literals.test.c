#include "unity.h"

#include "assert_value.h"
#include "ast_expr.h"
#include "eval.h"
#include "eval_unary.h"
#include "make_runtime_err.h"
#include "make_values.h"
#include "require_t.h"
#include "runtime_err.h"
#include "value_t.h"

void test_should_evaluate_string_literal(void)
{
	StringExpr str_expr = {.base = {.type = EXPR_LITERAL_STRING},
						   .value = "Hello, World!"};
	RuntimeError err;

	Value result = eval_expr((Expr*)&str_expr, &err);

	TEST_ASSERT_STRING_VALUE(result, "Hello, World!");
}

void test_should_evaluate_number_literal(void)
{
	NumberExpr num_expr = {.base = {.type = EXPR_LITERAL_NUMBER},
						   .value = "42.0"};
	RuntimeError err;

	Value result = eval_expr((Expr*)&num_expr, &err);

	TEST_ASSERT_NUMBER_VALUE(result, 42.0);
}

void test_should_evaluate_boolean_literal_true(void)
{
	BooleanExpr bool_expr = {.base = {.type = EXPR_LITERAL_BOOL}, .value = 1};
	RuntimeError err;

	Value result = eval_expr((Expr*)&bool_expr, &err);

	TEST_ASSERT_BOOL_VALUE(result, 1);
}
