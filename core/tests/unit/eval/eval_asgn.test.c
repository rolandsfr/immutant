#include "eval_asgn.h"

#include "unity.h"

#include "assert_value.h"
#include "ast_expr.h"
#include "ast_make_expr.h"
#include "env.h"
#include "error.h"
#include "make_values.h"
#include "value_t.h"

#include "Mockeval_expr.h"

void test_eval_asgn_should_assign_variable_value(void)
{
	// Setup environment and define variable
	Env* env = env_new(NULL, ENV_IMPURE);
	Value initial_value = {0};

	env_define(env, "x", initial_value, MUTABLE);

	// Create AssignExpr for 'x = 42'

	Expr* value_expr = (Expr*)make_number_expr("42", 1);
	AssignExpr* assign_expr = make_assign_expr("x", value_expr, 1);

	Error error = {-1};
	eval_expr_ExpectAndReturn(value_expr, &error, env, make_number(42));

	Value asgn_val = eval_asgn(assign_expr, env, &error);
	TEST_ASSERT_NUMBER_VALUE(asgn_val, 42);
}

void test_eval_asgn_should_return_error_for_undefined_variable(void)
{
	// Setup environment without defining variable
	Env* env = env_new(NULL, ENV_IMPURE);

	// Create AssignExpr for 'y = 100'
	Expr* value_expr = (Expr*)make_number_expr("100", 1);
	AssignExpr* assign_expr = make_assign_expr("y", value_expr, 1);

	Error error = {-1};
	eval_expr_ExpectAndReturn(value_expr, &error, env, make_number(100));

	Value asgn_val = eval_asgn(assign_expr, env, &error);
	TEST_ASSERT_EQUAL_INT(RUNTIME_UNDEFINED_VARIABLE, error.type);
}
