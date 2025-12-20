#include "eval_logic.h"

#include "ast_expr.h"
#include "ast_make_expr.h"
#include "error.h"
#include "eval_expr.h"
#include "make_values.h"
#include "require_t.h"

#include "Mockeval_expr.h"
#include "Mocklexer.h"

void test_eval_logic_and_checks_full_binary_expr(void)
{
	Env* env = NULL;
	Error err = {.type = ERROR_NONE};

	LogicalExpr expr;
	expr.operator = TOKEN_AND;
	expr.left = (Expr*)make_boolean_expr(1, 1);
	expr.right = (Expr*)make_boolean_expr(0, 1);

	// The right side should be evaluated
	eval_expr_ExpectAndReturn(expr.left, &err, env, make_bool(1));
	eval_expr_ExpectAndReturn(expr.right, &err, env, make_bool(0));

	Value result = eval_logic(&expr, &err, env);

	TEST_ASSERT_EQUAL_INT(VAL_BOOL, result.type);
	TEST_ASSERT_EQUAL_INT(0, result.boolean);

	free_expr(expr.left);
	free_expr(expr.right);
}

void test_eval_logic_and_yield_true_value(void)
{
	Env* env = NULL;
	Error err = {.type = ERROR_NONE};

	LogicalExpr expr;
	expr.operator = TOKEN_AND;
	expr.left = (Expr*)make_boolean_expr(1, 1);
	expr.right = (Expr*)make_boolean_expr(1, 1);

	// The right side should be evaluated
	eval_expr_ExpectAndReturn(expr.left, &err, env, make_bool(1));
	eval_expr_ExpectAndReturn(expr.right, &err, env, make_bool(1));

	Value result = eval_logic(&expr, &err, env);

	TEST_ASSERT_EQUAL_INT(VAL_BOOL, result.type);
	TEST_ASSERT_EQUAL_INT(1, result.boolean);

	// Clean up
	free_expr(expr.left);
	free_expr(expr.right);
}

void test_eval_logic_or_checks_full_binary(void)
{
	Env* env = NULL;
	Error err = {.type = ERROR_NONE};

	LogicalExpr expr;
	expr.operator = TOKEN_OR;
	expr.left = (Expr*)make_boolean_expr(0, 1);
	expr.right = (Expr*)make_boolean_expr(0, 1);

	// The right side should be evaluated
	eval_expr_ExpectAndReturn(expr.left, &err, env, make_bool(0));
	eval_expr_ExpectAndReturn(expr.right, &err, env, make_bool(0));

	Value result = eval_logic(&expr, &err, env);

	TEST_ASSERT_EQUAL_INT(VAL_BOOL, result.type);
	TEST_ASSERT_EQUAL_INT(0, result.boolean);

	// Clean up
	free_expr(expr.left);
	free_expr(expr.right);
}

void test_eval_logic_or_yields_true(void)
{
	Env* env = NULL;
	Error err = {.type = ERROR_NONE};

	LogicalExpr expr;
	expr.operator = TOKEN_OR;
	expr.left = (Expr*)make_boolean_expr(1, 1);
	expr.right = (Expr*)make_boolean_expr(0, 1);

	// The right side should be evaluated
	eval_expr_ExpectAndReturn(expr.left, &err, env, make_bool(1));

	Value result = eval_logic(&expr, &err, env);

	TEST_ASSERT_EQUAL_INT(VAL_BOOL, result.type);
	TEST_ASSERT_EQUAL_INT(1, result.boolean);

	// Clean up
	free_expr(expr.left);
	free_expr(expr.right);
}

// TODO: Write short cicruit tests once functions are in place and checking for
// evaluation is possible (via function call sideffects)
