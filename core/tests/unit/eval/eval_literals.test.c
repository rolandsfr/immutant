#include "unity.h"

#include "assert_value.h"
#include "ast_expr.h"
#include "env.h"
#include "eval_asgn.h"
#include "eval_binary.h"
#include "eval_call.h"
#include "eval_expr.h"
#include "eval_logic.h"
#include "eval_unary.h"
#include "eval_var.h"
#include "is_equal.h"
#include "make_values.h"
#include "require_t.h"
#include "value_t.h"

void test_should_evaluate_string_literal(void)
{
	StringExpr str_expr = {.base = {.type = EXPR_LITERAL_STRING},
						   .value = "Hello, World!"};

	Value result = eval_expr((Expr*)&str_expr, NULL, NULL);

	TEST_ASSERT_STRING_VALUE(result, "Hello, World!");
}

void test_should_evaluate_number_literal(void)
{
	NumberExpr num_expr = {.base = {.type = EXPR_LITERAL_NUMBER},
						   .value = "42.0"};

	Value result = eval_expr((Expr*)&num_expr, NULL, NULL);

	TEST_ASSERT_NUMBER_VALUE(result, 42.0);
}

void test_should_evaluate_boolean_literal_true(void)
{
	BooleanExpr bool_expr = {.base = {.type = EXPR_LITERAL_BOOL}, .value = 1};

	Value result = eval_expr((Expr*)&bool_expr, NULL, NULL);

	TEST_ASSERT_BOOL_VALUE(result, 1);
}
