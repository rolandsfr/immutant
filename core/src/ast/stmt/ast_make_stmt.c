#include "ast_make_stmt.h"

#include <stdlib.h>
#include <string.h>

#include "ast_stmt.h"

#include "../ast_cnstrct.h"

/** Statement factories
 * ========================
 */

VarDeclStmt* make_var_decl_stmt(const char* name, Expr* initializer,
								enum MutabilityType mutability)
{
	VarDeclStmt* v = malloc(sizeof(VarDeclStmt));
	v->base.type = STMT_VAR_DECL;
	v->name = strdup(name);
	v->initializer = initializer;
	v->mutability = mutability;
	return v;
}

FunDeclStmt* make_fun_decl_stmt(const char* name, char** param_names,
								size_t param_count, Stmt* body)
{
	FunDeclStmt* f = malloc(sizeof(FunDeclStmt));
	f->base.type = STMT_FUN_DECL;
	f->name = strdup(name);
	f->param_names = param_names;
	f->param_count = param_count;
	f->body = body;
	return f;
}

ReturnStmt* make_return_stmt(Expr* value)
{
	ReturnStmt* r = malloc(sizeof(ReturnStmt));
	r->base.type = STMT_RETURN;
	r->value = value;
	return r;
}

BlockStmt* make_block_stmt(Stmt** statements, size_t count)
{
	BlockStmt* b = malloc(sizeof(BlockStmt));
	b->base.type = STMT_BLOCK;
	b->statements = statements;
	b->count = count;
	return b;
}

IfStmt* make_if_stmt(Expr* condition, Stmt* then_branch, Stmt* else_branch)
{
	IfStmt* i = malloc(sizeof(IfStmt));
	i->base.type = STMT_IF;
	i->condition = condition;
	i->then_branch = then_branch;
	i->else_branch = else_branch;
	return i;
}

ExprStmt* make_expr_stmt(Expr* expression)
{
	ExprStmt* e = malloc(sizeof(ExprStmt));
	e->base.type = STMT_EXPR;
	e->expression = expression;
	return e;
}
