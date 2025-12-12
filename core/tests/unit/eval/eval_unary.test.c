#include "eval_unary.h"

#include "unity.h"

#include "assert_value.h"
#include "ast_expr.h"
#include "ast_make_expr.h"
#include "error.h"
#include "eval.h"
#include "eval_binary.h"
#include "is_equal.h"
#include "make_values.h"
#include "require_t.h"
#include "value_t.h"

void test_eval_unary_should_not_work_with_strings(void)
{
	UnaryExpr* unary_expr =
		make_unary_expr(TOKEN_MINUS, (Expr*)make_string_expr("hello"));
	Error err = {-1};

	Value result = eval_unary(unary_expr, &err);

	TEST_ASSERT_RUNTIME_ERROR(&err, RUNTIME_UNEXPECTED_TYPE);
}

void test_eval_unary_should_negate_number(void)
{
	UnaryExpr* expr =
		make_unary_expr(TOKEN_MINUS, (Expr*)make_number_expr("10"));
	Error err = {-1};

	Value result = eval_unary(expr, &err);

	TEST_ASSERT_NUMBER_VALUE(result, -10.0);
}

void test_eval_unary_should_negate_boolean(void)
{
	UnaryExpr* expr = make_unary_expr(TOKEN_BANG, (Expr*)make_boolean_expr(1));
	Error err = {-1};
	Value result = eval_unary(expr, &err);

	TEST_ASSERT_BOOL_VALUE(result, 0);
}

void test_eval_unary_should_eval_binary_inside_unary(void)
{

	BinaryExpr* bin = make_binary_expr((Expr*)make_number_expr("5"), TOKEN_PLUS,
									   (Expr*)make_number_expr("3"));

	UnaryExpr* expr = make_unary_expr(TOKEN_MINUS, (Expr*)bin);

	Error err = {-1};

	Value result = eval_unary(expr, &err);

	TEST_ASSERT_NUMBER_VALUE(result, -8.0);
}

void test_eval_unary_should_negate_boolean_false(void)
{
	UnaryExpr* expr = make_unary_expr(TOKEN_BANG, (Expr*)make_boolean_expr(0));
	Error err = {-1};
	Value result = eval_unary(expr, &err);

	TEST_ASSERT_BOOL_VALUE(result, 1);
}
