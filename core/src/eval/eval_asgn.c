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

	if (env_entry.name == NULL) {

		if (out_error) {
			*out_error = (Error){.type = RUNTIME_UNDEFINED_VARIABLE,
								 .line = expr->base.line,
								 .message = ""};
			snprintf(out_error->message, sizeof(out_error->message),
					 "Undefined variable: '%s'", expr->name);
		}
		return (Value){0};
	}
	// printf("value type to assign: %d\n", val.type);
	// printf("variable '%s' mutability: %d\n", expr->name,
	// 	   env_entry.value.mutability);

	switch (env_entry.value.mutability) {
		case MUTABILITY_UNSET: {
			if (out_error) {
				*out_error = (Error){.type = RUNTIME_UNDEFINED_VARIABLE,
									 .line = expr->base.line,
									 .message = ""};
				snprintf(out_error->message, sizeof(out_error->message),
						 "Variable '%s' has unset mutability", expr->name);
			}
		}

		case MUTABLE: {

			// preverve original var mutability...
			// because literal make functions set MUTABILITY_UNSET, need to make
			// additional effort to preserve mutability here

			enum MutabilityType mut =
				env_get_entry(env, expr->name).value.mutability;

			env_set(env, expr->name, val);
			Value* val = env_get(env, expr->name);
			val->mutability = mut;

			break;
		}
		case IMMUTABLE:
			if (out_error) {
				*out_error = (Error){.type = RUNTIME_ASSIGN_TO_CONSTANT,
									 .line = expr->base.line,
									 .message = ""};
				snprintf(out_error->message, sizeof(out_error->message),
						 "Cannot assign to constant variable: '%s'",
						 expr->name);
			}
			return (Value){0};
			break;
	}

	return val;
}
