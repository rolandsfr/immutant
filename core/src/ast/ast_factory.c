#include <stdlib.h>
#include <string.h>

#include "ast.h"

/** Expression factories
 * ========================
 */

NumberExpr* make_number_expr(double value)
{
	NumberExpr* n = malloc(sizeof(NumberExpr));
	n->base.type = EXPR_LITERAL_NUMBER;
	n->value = value;
	return n;
}

BinaryExpr* make_binary_expr(Expr* left, char op, Expr* right)
{
	BinaryExpr* b = malloc(sizeof(BinaryExpr));
	b->base.type = EXPR_BINARY;
	b->left = left;
	b->operator = op;
	b->right = right;
	return b;
}

StringExpr* make_string_expr(const char* value)
{
	StringExpr* s = malloc(sizeof(StringExpr));
	s->base.type = EXPR_LITERAL_STRING;
	s->value = strdup(value);
	return s;
}

VariableExpr* make_variable_expr(const char* name)
{
	VariableExpr* v = malloc(sizeof(VariableExpr));
	v->base.type = EXPR_VARIABLE;
	v->name = strdup(name);
	return v;
}

UnaryExpr* make_unary_expr(char operator, Expr* operand)
{
	UnaryExpr* u = malloc(sizeof(UnaryExpr));
	u->base.type = EXPR_UNARY;
	u->operator = operator;
	u->operand = operand;
	return u;
}

CallExpr* make_call_expr(const char* name, Expr** args, size_t arg_count)
{
	CallExpr* c = malloc(sizeof(CallExpr));
	c->base.type = EXPR_CALL;
	c->name = strdup(name);
	c->args = args;
	c->arg_count = arg_count;
	return c;
}

/** Statement factories
 * ========================
 */

VarDeclStmt* make_var_decl_stmt(const char* name, Expr* initializer)
{
	VarDeclStmt* v = malloc(sizeof(VarDeclStmt));
	v->base.type = STMT_VAR_DECL;
	v->name = strdup(name);
	v->initializer = initializer;
	return v;
}

ExprStmt* make_expr_stmt(Expr* expression)
{
	ExprStmt* e = malloc(sizeof(ExprStmt));
	e->base.type = STMT_EXPR;
	e->expression = expression;
	return e;
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

/** Resource deallocation
 * ========================
 */

void free_expr(Expr* expr)
{
	if (!expr)
		return;

	switch (expr->type) {
		case EXPR_LITERAL_NUMBER: {
			free(expr);
			break;
		}
		case EXPR_LITERAL_STRING: {
			StringExpr* s = (StringExpr*)expr;
			free(s->value);
			free(s);
			break;
		}
		case EXPR_VARIABLE: {
			VariableExpr* v = (VariableExpr*)expr;
			free(v->name);
			free(v);
			break;
		}
		case EXPR_UNARY: {
			UnaryExpr* u = (UnaryExpr*)expr;
			free_expr(u->operand);
			free(u);
			break;
		}
		case EXPR_BINARY: {
			BinaryExpr* b = (BinaryExpr*)expr;
			free_expr(b->left);
			free_expr(b->right);
			free(b);
			break;
		}
		case EXPR_CALL: {
			CallExpr* c = (CallExpr*)expr;
			for (size_t i = 0; i < c->arg_count; i++) {
				free_expr(c->args[i]);
			}
			free(c->args);
			free(c->name);
			free(c);
			break;
		}
	}
}
