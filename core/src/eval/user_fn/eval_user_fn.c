#include "eval_user_fn.h"

#include "ast_stmt.h"
#include "env.h"
#include "eval_block_stmt.h"
#include "make_values.h"
#include "value_t.h"

// sort of a little eval() exposed to the user
Value call_user_fn(ValueBuffer* arguments, Context* context)
{
	FunDeclStmt* declaration = context->declaration;

	Env* fn_env =
		env_new(context->env,
				context->declaration->purity == PURE ? ENV_PURE : ENV_IMPURE);
	// pure function re-defines all the variables inside of it, because it's
	// prohibited to change any external state. additionally it knows nothing
	// about external state, so that it cannot change it (so no global
	// variables)

	for (size_t i = 0; i < declaration->params->len; i++) {
		const char* param_name = declaration->params->data[i];
		Value arg_value = arguments->data[i];

		/** Requirements for function scope:
		 *
		 * --- Pure functions define ALL passed values in their own scope,
		 *   not to be able to change external state.
		 *
		 *   All of the passed values are defined as MUTABLE variables,
		 *   because of usage convenience. This part may eventually change in
		 * language specification.
		 *
		 *
		 * --- Impure functions should reach all passed values as they are,
		 *   to be able to reach out to the external state.
		 *
		 * */
		EnvEntry existing_entry = env_get_entry(fn_env, param_name);

		if (context->declaration->purity == PURE || !existing_entry.name) {
			env_define(fn_env, param_name, arg_value, MUTABLE);
		}
	}

	// TODO: free return_value after use
	Value* return_value = malloc(sizeof(Value));
	eval_block_stmt(context->declaration->body, context->error_out_tunnel,
					fn_env, return_value);
	return *return_value;
}

Value create_user_fn(Env* env, FunDeclStmt* declaration)
{
	Callable* user_callable = malloc(sizeof(Callable));
	user_callable->arity = declaration->params->len;
	user_callable->call = call_user_fn;
	user_callable->declaration = declaration;

	Value user_callable_value;
	user_callable_value.type = VAL_FN;
	user_callable_value.callable = user_callable;

	return user_callable_value;
}
