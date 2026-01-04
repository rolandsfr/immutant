
#include <stdlib.h>
#include <string.h>

#include "ast_cnstrct.h"
#include "ast_expr.h"
#include "ast_stmt.h"
#include "error.h"
#include "eval_binary.h"
#include "eval_block_stmt.h"
#include "eval_expr.h"
#include "eval_if_stmt.h"
#include "eval_unary.h"
#include "eval_while_stmt.h"
#include "make_values.h"
#include "value_t.h"

#include "eval_vars/eval_var.h"
#include "user_fn/def_user_fn.h"

void eval(Stmt* stmt, Error* err, Value* out_value, Env* env)
{
	switch (stmt->type) {
		case STMT_EXPR: {
			ExprStmt* expr_stmt = (ExprStmt*)stmt;
			*out_value = eval_expr(expr_stmt->expression, err, env);
			return;
		}
		case STMT_VAR_DECL: {
			eval_var_dclr(env, (VarDeclStmt*)stmt, err);
			return;
		}
		case STMT_FUN_DECL: {
			def_user_fn((FunDeclStmt*)stmt, env, err);
			return;
		}
		case STMT_BLOCK: {
			eval_block_stmt((BlockStmt*)stmt, err, env, out_value);
			return;
		}
		case STMT_IF: {
			eval_if_stmt((IfStmt*)stmt, err, env, out_value);
			return;
		}
		case STMT_WHILE: {
			eval_while_stmt((WhileStmt*)stmt, err, env, out_value);
			return;
		}
	}

	fprintf(stderr, "Internal interpreter error: unknown expression type\n");
}
