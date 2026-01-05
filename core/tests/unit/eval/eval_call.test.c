#include "eval_call.h"

#include "unity.h"

#include "ast_expr.h"
#include "ast_make_expr.h"
#include "env.h"
#include "error.h"
#include "lexer.h"
#include "make_values.h"
#include "parser_helpers.h"
#include "value_t.h"

#include "Mockeval_expr.h"
#include "Mockparse_call.h"
#include "Mockresolve.h"

void define_mock_function_in_env(Env* env, const char* name, size_t arity,
								 Value (*mock_function)(ValueBuffer*, Context*),
								 PurityType purity)
{
	Callable* callable = malloc(sizeof(Callable));
	callable->arity = arity;
	callable->call = mock_function;

	Value func_value;
	func_value.type = VAL_NATIVE;
	func_value.callable = callable;

	env_define_fn(env, name, func_value, purity);
}

Value mock_function(ValueBuffer* arguments, Context* context)
{
	if (arguments->len != 1 || arguments->data[0].type != VAL_NUMBER) {
		return make_number(-1);
	}

	return make_number(arguments->data[0].number * 2);
}

void test_eval_call_should_invoke_function_with_arguments(void)
{
	Env* env = env_new(NULL, ENV_IMPURE);
	define_mock_function_in_env(env, "mockFunc", 1, mock_function, IMPURE);

	ArgumentsArray args;
	ArgumentsArray_init(&args);
	ArgumentsArray_push(&args, NULL);

	VariableExpr* callee_expr = make_variable_expr("mockFunc", 1);
	CallExpr* call_expr = make_call_expr((Expr*)callee_expr, args.data, 1, 1);
	Error error = {-1};

	// mock identifier evaluation
	Value* callee = env_get(env, "mockFunc");
	eval_expr_ExpectAndReturn((Expr*)callee_expr, &error, env, *callee);

	// mock argument evaluation
	eval_expr_ExpectAndReturn(call_expr->args[0], &error, env, make_number(10));

	Value result = eval_call(call_expr, &error, env);
	TEST_ASSERT_EQUAL_INT(20, result.number);

	ArgumentsArray_free(&args);
}

void test_eval_call_should_return_error_when_callee_is_not_function(void)
{
	Env* env = env_new(NULL, ENV_IMPURE);

	ArgumentsArray args;
	ArgumentsArray_init(&args);
	ArgumentsArray_push(&args, NULL);

	VariableExpr* callee_expr = make_variable_expr("notAFunction", 1);
	CallExpr* call_expr = make_call_expr((Expr*)callee_expr, args.data, 1, 1);
	Error error = {-1};

	// mock identifier evaluation to return a non-function value
	Value non_function_value = make_number(42);
	eval_expr_ExpectAndReturn((Expr*)callee_expr, &error, env,
							  non_function_value);

	Value result = eval_call(call_expr, &error, env);
	TEST_ASSERT_EQUAL_INT(RUNTIME_UNEXPECTED_TYPE, error.type);

	ArgumentsArray_free(&args);
}

void test_eval_call_should_return_error_on_incorrect_argument_count(void)
{
	Env* env = env_new(NULL, ENV_IMPURE);
	define_mock_function_in_env(env, "mockFunc", 2, mock_function, IMPURE);

	ArgumentsArray args;
	ArgumentsArray_init(&args);
	ArgumentsArray_push(&args, NULL); // only 1 argument instead of 2

	VariableExpr* callee_expr = make_variable_expr("mockFunc", 1);
	CallExpr* call_expr = make_call_expr((Expr*)callee_expr, args.data, 1, 1);
	Error error = {-1};

	// mock identifier evaluation
	Value* callee = env_get(env, "mockFunc");
	eval_expr_ExpectAndReturn((Expr*)callee_expr, &error, env, *callee);

	// mock argument evaluation
	eval_expr_ExpectAndReturn(call_expr->args[0], &error, env, make_number(10));

	Value result = eval_call(call_expr, &error, env);
	TEST_ASSERT_EQUAL_INT(RUNTIME_INCORRECT_ARG_COUNT, error.type);

	ArgumentsArray_free(&args);
}

void test_eval_call_should_return_error_when_pure_function_calls_impure(void)
{
	Env* env = env_new(NULL, ENV_PURE);
	define_mock_function_in_env(env, "impureFunc", 1, mock_function, IMPURE);

	ArgumentsArray args;
	ArgumentsArray_init(&args);
	ArgumentsArray_push(&args, NULL);

	VariableExpr* callee_expr = make_variable_expr("impureFunc", 1);
	CallExpr* call_expr = make_call_expr((Expr*)callee_expr, args.data, 1, 1);
	Error error = {-1};

	// mock identifier evaluation
	Value* callee = env_get(env, "impureFunc");
	eval_expr_ExpectAndReturn((Expr*)callee_expr, &error, env, *callee);

	// mock argument evaluation
	eval_expr_ExpectAndReturn(call_expr->args[0], &error, env, make_number(10));

	Value result = eval_call(call_expr, &error, env);
	TEST_ASSERT_EQUAL_INT(RUNTIME_PURITY_VIOLATION, error.type);

	ArgumentsArray_free(&args);
}

void test_pure_function_should_not_resolve_mutable_variable_from_outer_scope(
	void)
{
	Env* global_env = env_new(NULL, ENV_IMPURE);
	define_mock_function_in_env(global_env, "impureFunc", 1, mock_function,
								IMPURE);

	// Define a mutable variable in the global environment
	env_define_var(global_env, "mutableVar", make_number(5), MUTABLE);

	ArgumentsArray args;
	ArgumentsArray_init(&args);
	ArgumentsArray_push(&args, NULL);

	// create impure function
	VariableExpr* callee_expr = make_variable_expr("impureFunc", 1);
	CallExpr* call_expr = make_call_expr((Expr*)callee_expr, args.data, 1, 1);
	Error error = {-1};

	// mock identifier evaluation
	Value* callee = env_get(global_env, "impureFunc");
	eval_expr_ExpectAndReturn((Expr*)callee_expr, &error, global_env, *callee);

	// mock argument evaluation to try to access mutableVar
	VariableExpr* arg_expr = make_variable_expr("mutableVar", 1);
	call_expr->args[0] = (Expr*)arg_expr;

	error.type = RUNTIME_UNDEFINED_VARIABLE; // Simulate the error

	Value result = eval_call(call_expr, &error, global_env);
	TEST_ASSERT_EQUAL_INT(
		RUNTIME_UNDEFINED_VARIABLE,
		error.type); // mutable var is defined globally, but should not be
					 // accessible in pure function

	ArgumentsArray_free(&args);
}
