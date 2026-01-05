
#include "eval_return_stmt.h"

#include "ast_cnstrct.h"
#include "ast_stmt.h"
#include "env.h"
#include "error.h"
#include "eval.h"
#include "eval_expr.h"
#include "require_t.h"
#include "value_t.h"

void eval_return_stmt(ReturnStmt* stmt, Error* err, Env* env)
{

	if (stmt->value) {
		Value return_value = eval_expr(stmt->value, err, env);

		if (err->type != ERROR_NONE) {

			printf("Error occurred during return value evaluation: %s\n",
				   err->message);
			return;
		}

		err->return_value = return_value;
	}

	err->type = RUNTIME_RETURN_ERROR;
	err->line = stmt->base.line;
	strcpy(err->message, "Unexpected return statement");
}
