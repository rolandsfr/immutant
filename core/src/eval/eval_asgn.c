#include "eval_asgn.h"

#include "ast_expr.h"
#include "ast_stmt.h"
#include "error.h"
#include "eval_expr.h"
#include "make_values.h"
#include "value_t.h"

#include "env/env.h"

Value eval_asgn(AssignExpr* expr, Env* env, Error* out_error)
{
	Value val = eval_expr(expr->value, out_error, env);

	if (out_error && out_error->type != ERROR_NONE) {
		return (Value){0};
	}

	EnvEntry env_entry = env_get_entry(env, expr->name);

    if(env_entry.name == NULL) { 

		if (out_error) {
			*out_error = (Error){.type = RUNTIME_UNDEFINED_VARIABLE,
								 .line = expr->base.line,
								 .message = ""};
			snprintf(out_error->message, sizeof(out_error->message),
					 "Undefined variable: '%s'", expr->name);
		}
		return (Value){0};
    }

    switch(env_entry.mutability) {
        case MUTABLE:
            env_set(env, expr->name, val);
            break;
        case IMMUTABLE:
            if (out_error) {
                *out_error = (Error){.type = RUNTIME_ASSIGN_TO_CONSTANT,
                                     .line = expr->base.line,
                                     .message = ""};
                snprintf(out_error->message, sizeof(out_error->message),
                         "Cannot assign to constant variable: '%s'", expr->name);
            }
            return (Value){0};
            break;
    }


	return val;
}
