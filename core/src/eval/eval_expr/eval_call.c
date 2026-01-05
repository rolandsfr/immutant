

#include <stdlib.h>
#include <string.h>

#include "ast_expr.h"
#include "env.h"
#include "error.h"
#include "eval_expr.h"
#include "eval_singature.h"
#include "lexer.h"
#include "make_values.h"
#include "parser_singnature.h"
#include "require_t.h"
#include "value_t.h"

DEF_EVAL_EXPR(eval_call, CallExpr)
{
	// TODO: if this call is within another fn call which isnt impure, and this
	// is pure, immediately error
	// "Pure function cannot call impure function“

	Value callee = eval_expr((Expr*)expr->callee, err, env);

	if (err && err->type != ERROR_NONE) {
		return make_null();
	}

	if (callee.type != VAL_FN && callee.type != VAL_NATIVE) {
		if (err) {
			// TODO: would be nice to include function name in error message
			*err = (Error){.type = RUNTIME_UNEXPECTED_TYPE,
						   .line = expr->base.line,
						   .message = "Callee is not a function"};
		}
		return make_null();
	}

	ValueBuffer args_buffer;
	ValueBuffer_init(&args_buffer);

	Callable* callable_val = callee.callable;

	for (size_t i = 0; i < expr->arg_count; i++) {
		Value arg_value = eval_expr(expr->args[i], err, env);
		if (err && err->type != ERROR_NONE) {
			ValueBuffer_free(&args_buffer);
			return make_null();
		}

		ValueBuffer_push(&args_buffer, arg_value);
	}

	if (args_buffer.len != callable_val->arity) {
		if (err) {
			*err = (Error){
				.type = RUNTIME_INCORRECT_ARG_COUNT,
				.line = expr->base.line,
				.message = "Incorrect number of arguments passed to function"};
		}
		ValueBuffer_free(&args_buffer);
		return make_null();
	}

	if (callee.purity == IMPURE && env->purity == ENV_PURE) {
		if (err) {
			*err =
				(Error){.type = RUNTIME_PURITY_VIOLATION,
						.line = expr->base.line,
						.message = "Pure function cannot call impure function"};
		}
		ValueBuffer_free(&args_buffer);
		return make_null();
	}

	Error saved_error = *err;

	Value result = callable_val->call(
		&args_buffer, &(Context){.line = expr->base.line,
								 .error_out_tunnel = err,
								 .declaration = callable_val->declaration,
								 .env = env});

	if (err->type == RUNTIME_RETURN_ERROR) {
		result = err->return_value;
		*err = saved_error; // restore outer error state
	}

	if (err->type != ERROR_NONE) {
		ValueBuffer_free(&args_buffer);
		return make_null();
	}

	if (err && err->type != ERROR_NONE) {
		ValueBuffer_free(&args_buffer);
		return make_null();
	}

	ValueBuffer_free(&args_buffer);
	return result;
}
