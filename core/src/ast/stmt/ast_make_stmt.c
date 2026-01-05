#include "ast_make_stmt.h"

#include <stdlib.h>
#include <string.h>

#include "ast_stmt.h"
#include "value_t.h"

#include "../ast_cnstrct.h"

/** Statement factories
 * ========================
 */

VarDeclStmt* make_var_decl_stmt(const char* name, Expr* initializer,
								enum MutabilityType mutability, int line_nr)
{
	VarDeclStmt* v = malloc(sizeof(VarDeclStmt));
	v->base.type = STMT_VAR_DECL;
	v->base.line = line_nr;
	v->name = strdup(name);
	v->initializer = initializer;
	v->mutability = mutability;
	return v;
}
FunDeclStmt* make_fun_decl_stmt(const char* name, ParamArray* params,

								size_t param_count, BlockStmt* body,
								PurityType purity, int line_nr)
{
	FunDeclStmt* f = malloc(sizeof(FunDeclStmt));
	f->base.type = STMT_FUN_DECL;
	f->base.line = line_nr;
	f->params = params;
	f->purity = purity;
	f->name = strdup(name);
	f->body = body;
	return f;
}
ReturnStmt* make_return_stmt(Expr* value, int line_nr)
{
	ReturnStmt* r = malloc(sizeof(ReturnStmt));
	r->base.type = STMT_RETURN;
	r->base.line = line_nr;
	r->value = value;
	return r;
}

BlockStmt* make_block_stmt(Stmt** statements, size_t count, int line_nr)
{
	BlockStmt* b = malloc(sizeof(BlockStmt));
	b->base.type = STMT_BLOCK;
	b->base.line = line_nr;
	b->statements = statements;
	b->count = count;
	return b;
}

IfStmt* make_if_stmt(Expr* condition, Stmt* then_branch, Stmt* else_branch,
					 int line_nr)
{
	IfStmt* i = malloc(sizeof(IfStmt));
	i->base.type = STMT_IF;
	i->base.line = line_nr;
	i->condition = condition;
	i->then_branch = then_branch;
	i->else_branch = else_branch;
	return i;
}

WhileStmt* make_while_stmt(Expr* condition, Stmt* body, int line_nr)
{
	WhileStmt* w = malloc(sizeof(WhileStmt));
	w->base.type = STMT_WHILE;
	w->base.line = line_nr;
	w->condition = condition;
	w->body = body;
	return w;
}

ExprStmt* make_expr_stmt(Expr* expression, int line_nr)
{
	ExprStmt* e = malloc(sizeof(ExprStmt));
	e->base.type = STMT_EXPR;
	e->base.line = line_nr;
	e->expression = expression;
	return e;
}
