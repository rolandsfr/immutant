#include "eval_while_stmt.h"

#include "ast_cnstrct.h"
#include "ast_stmt.h"
#include "env.h"
#include "error.h"
#include "eval.h"
#include "eval_expr.h"
#include "require_t.h"
#include "value_t.h"

void eval_while_stmt(WhileStmt* stmt, Error* err, Env* env, Value* out_value)
{
	Value condition = eval_expr(stmt->condition, err, env);

	if (!require_bool(condition, err))
		return;

	while (condition.boolean) {
		eval(stmt->body, err, out_value, env);
		condition =
			eval_expr(stmt->condition, err, env); // re-evaluate condition

		if (!require_bool(condition, err))
			break;
	}
}
