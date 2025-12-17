#include "eval_if_stmt.h"

#include "ast_stmt.h"
#include "env.h"
#include "error.h"
#include "require_t.h"
#include "value_t.h"

#include "Mockeval.h"
#include "Mockeval_expr.h"

void test_eval_if_stmt_executes_then_branch_when_condition_is_true()
{
	// Arrange
	IfStmt stmt;
	Value condition = {.type = VAL_BOOL, .boolean = 1};
	stmt.condition = NULL; // Not used in this test

	Stmt then_branch;
	Stmt else_branch;
	stmt.then_branch = &then_branch;
	stmt.else_branch = &else_branch;

	Env env;
	Error err;
	Value out_value;

	// Set up the mock for eval_expr to return true
	eval_expr_ExpectAndReturn(stmt.condition, &err, &env, condition);
	eval_Expect(&then_branch, &err, &out_value, &env);

	// Act
	eval_if_stmt(&stmt, &err, &env, &out_value);
}

void test_eval_if_stmt_executes_else_branch_when_condition_is_not_true()
{
	// Arrange
	IfStmt stmt;
	Value condition = {.type = VAL_BOOL, .boolean = 0};
	stmt.condition = NULL; // Not used in this test

	Stmt then_branch;
	Stmt else_branch;
	stmt.then_branch = &then_branch;
	stmt.else_branch = &else_branch;

	Env env;
	Error err;
	Value out_value;

	// Set up the mock for eval_expr to return true
	eval_expr_ExpectAndReturn(stmt.condition, &err, &env, condition);
	eval_Expect(&else_branch, &err, &out_value, &env);

	// Act
	eval_if_stmt(&stmt, &err, &env, &out_value);
}
