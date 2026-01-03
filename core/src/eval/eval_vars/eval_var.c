#include <stdio.h>

#include "ast_expr.h"
#include "ast_stmt.h"
#include "error.h"
#include "eval.h"
#include "eval_expr.h"
#include "value_t.h"

#include "env/env.h"

void eval_var_dclr(Env* env, VarDeclStmt* var_stmt, Error* error)
{
	Value value;
	if (var_stmt->initializer != NULL) {
		value = eval_expr(var_stmt->initializer, error, env);
	}

	if (env_get_direct_entry(env, var_stmt->name) != NULL) {
		if (error) {
			*error = (Error){.type = RUNTIME_VARIABLE_REDEFINITION,
							 .line = var_stmt->base.line,
							 .message = ""};
			snprintf(error->message, sizeof(error->message),
					 "Variable already defined: '%s'", var_stmt->name);
		}
		return;
	}

	env_define(env, var_stmt->name, value, var_stmt->mutability);
}

void eval_var_expr(Env* env, VariableExpr* var_expr, Value* out_value,
				   Error* error)
{
	Value* value = env_get(env, var_expr->name);
	if (value == NULL) {
		if (error) {
			*error = (Error){.type = RUNTIME_UNDEFINED_VARIABLE,
							 .line = var_expr->base.line,
							 .message = ""};
			snprintf(error->message, sizeof(error->message),
					 "Undefined variable: '%s'", var_expr->name);
		}
		return;
	}

	*out_value = *value;
}
