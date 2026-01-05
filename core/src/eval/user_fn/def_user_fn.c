#include "def_user_fn.h"

#include "env.h"
#include "error.h"
#include "eval_user_fn.h"
#include "value_t.h"

void def_user_fn(FunDeclStmt* declaration, Env* env, Error* err)
{
	// allow only global functions
	if (env->parent != NULL) {

		if (err) {
			*err = (Error){.type = RUNTIME_INVALID_FUNCTION_DEFINITION,
						   .line = declaration->base.line};

			snprintf(err->message, sizeof(err->message),
					 "Function '%s' can only be defined in the global scope",
					 declaration->name);

			return;
		}
	}

	Value fn_value = create_user_fn(env, declaration);
	env_define_fn(env, declaration->name, fn_value, declaration->purity);
}
