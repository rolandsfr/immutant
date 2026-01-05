#include "eval_if_stmt.h"

#include "ast_cnstrct.h"
#include "ast_stmt.h"
#include "env.h"
#include "error.h"
#include "eval.h"
#include "eval_expr.h"
#include "require_t.h"
#include "value_t.h"

void eval_if_stmt(IfStmt* stmt, Error* err, Env* env, Value* out_value)
{
	Value condition = eval_expr(stmt->condition, err, env);

	if (!require_bool(condition, err, stmt->condition->line))
		return;

	if (condition.boolean) {
		return eval(stmt->then_branch, err, out_value, env);
	} else if (stmt->else_branch) {
		return eval(stmt->else_branch, err, out_value, env);
	}
}
