#include "eval_unary.h"

#include "unity.h"

#include "assert_value.h"
#include "ast_expr.h"
#include "ast_make_expr.h"
#include "env.h"
#include "error.h"
#include "is_equal.h"
#include "make_values.h"
#include "require_t.h"
#include "value_t.h"

#include "Mockeval_expr.h"

void test_eval_unary_should_not_work_with_strings(void)
{
	UnaryExpr* unary_expr =
		make_unary_expr(TOKEN_MINUS, (Expr*)make_string_expr("hello"));
	Error err = {-1};

	eval_expr_IgnoreAndReturn(make_string("hello"));

	Value result = eval_unary(unary_expr, &err, NULL);

	TEST_ASSERT_RUNTIME_ERROR(&err, RUNTIME_UNEXPECTED_TYPE);
}

void test_eval_unary_should_negate_number(void)
{
	UnaryExpr* expr =
		make_unary_expr(TOKEN_MINUS, (Expr*)make_number_expr("10"));
	Error err = {-1};

	eval_expr_IgnoreAndReturn(make_number(10));

	Value result = eval_unary(expr, &err, NULL);

	TEST_ASSERT_NUMBER_VALUE(result, -10.0);
}

void test_eval_unary_should_negate_boolean(void)
{
	UnaryExpr* expr = make_unary_expr(TOKEN_BANG, (Expr*)make_boolean_expr(1));
	Error err = {-1};

	eval_expr_IgnoreAndReturn(make_bool(1));

	Value result = eval_unary(expr, &err, NULL);

	TEST_ASSERT_BOOL_VALUE(result, 0);
}

void test_eval_unary_should_negate_boolean_false(void)
{
	UnaryExpr* expr = make_unary_expr(TOKEN_BANG, (Expr*)make_boolean_expr(0));
	Error err = {-1};
	eval_expr_IgnoreAndReturn(make_bool(0));
	Value result = eval_unary(expr, &err, NULL);

	TEST_ASSERT_BOOL_VALUE(result, 1);
}
