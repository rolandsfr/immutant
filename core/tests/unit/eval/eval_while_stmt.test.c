#include "eval_while_stmt.h"

#include "ast_stmt.h"
#include "env.h"
#include "error.h"
#include "require_t.h"
#include "value_t.h"

#include "Mockeval.h"
#include "Mockeval_expr.h"

void test_eval_while_stmt_executes_then_branch_when_condition_is_true()
{
	WhileStmt stmt;
	Stmt body;
	stmt.condition = &(Expr){.line = 1};
	stmt.body = &body;

	Env env;
	Error err = {-1};
	Value out_value;

	Value condition_true = {.type = VAL_BOOL, .boolean = 1};
	Value condition_false = {.type = VAL_BOOL, .boolean = 0};

	// call order matters

	eval_expr_ExpectAndReturn(stmt.condition, &err, &env, condition_true);
	eval_Expect(&body, &err, &out_value, &env);

	eval_expr_ExpectAndReturn(stmt.condition, &err, &env, condition_true);
	eval_Expect(&body, &err, &out_value, &env);

	eval_expr_ExpectAndReturn(stmt.condition, &err, &env, condition_true);
	eval_Expect(&body, &err, &out_value, &env);

	eval_expr_ExpectAndReturn(stmt.condition, &err, &env, condition_false);

	eval_while_stmt(&stmt, &err, &env, &out_value);
}
