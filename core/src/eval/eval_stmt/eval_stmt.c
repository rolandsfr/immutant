#include "ast_cnstrct.h"
#include "ast_stmt.h"
#include "error.h"
#include "value_t.h"

#include "eval_vars/eval_var.h"

void eval_stmt(Stmt* stmt, Error* err, Value* out_value, Env* env)
{
	switch (stmt->type) {
		case STMT_VAR_DECL: {
			eval_var_dclr(env, (VarDeclStmt*)stmt, err);
			return;
		}
	}
}
