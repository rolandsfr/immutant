#include "eval_block_stmt.h"

#include "unity.h"

#include "ast_make_expr.h"
#include "ast_make_stmt.h"
#include "ast_stmt.h"
#include "env.h"
#include "error.h"
#include "eval.h"
#include "lexer.h"
#include "make_values.h"
#include "parse_block_stmt.h"

#include "Mockeval_expr.h"
#include "Mockeval_var.h"
#include "Mockparse_dclr.h"
#include "Mockparser_helpers.h"
#include "Mockresolve.h"

Env* global_env;
void mock_eval_var_dclr(Env* env, VarDeclStmt* stmt, Error* err, int num_calls)
{
	TEST_ASSERT_EQUAL_PTR(global_env, env->parent);
}

void test_eval_block_stmt_creates_new_scope(void)
{
	global_env = env_new(NULL);

	Stmt* stmts[1];
	stmts[0] =
		(Stmt*)make_var_decl_stmt("x", (Expr*)make_number_expr("1"), MUTABLE);

	BlockStmt* block = make_block_stmt(stmts, 1);

	Error err = {0};
	Value out;

	eval_var_dclr_StubWithCallback(mock_eval_var_dclr);

	eval_block_stmt(block, &err, global_env, &out);

	env_free(global_env);
}
