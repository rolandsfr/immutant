#include "ast_cnstrct.h"
#include "ast_stmt.h"
#include "env.h"
#include "error.h"
#include "eval.h"
#include "value_t.h"

#include "eval_vars/eval_var.h"

void eval_block_stmt(BlockStmt* stmt, Error* err, Env* env, Value* out_value)
{
	Env* block_env = env_new(env, env->purity);

	for (size_t i = 0; i < stmt->count; i++) {
		Stmt* current_stmt = stmt->statements[i];
		eval(current_stmt, err, out_value, block_env);
	}

	env_free(block_env);
}
