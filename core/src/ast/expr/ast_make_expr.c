
#include "ast_make_expr.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ast_expr.h"
#include "lexer.h"

#include "../ast_cnstrct.h"

/** Expression factories
 * ========================
 */

BinaryExpr* make_binary_expr(Expr* left, enum TokenType op, Expr* right)
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

// TODO: change to accpeting and storing number instead of string once number
// parsing is implemented
NumberExpr* make_number_expr(const char* value)
{
	// NumberExpr* n = malloc(sizeof(NumberExpr));
	// n->base.type = EXPR_LITERAL_NUMBER;
	// n->value = value;
	// return n;

	NumberExpr* s = malloc(sizeof(NumberExpr));
	s->base.type = EXPR_LITERAL_NUMBER;
	s->value = strdup(value);
	return s;
}

BooleanExpr* make_boolean_expr(int value)
{
	BooleanExpr* b = malloc(sizeof(BooleanExpr));
	b->base.type = EXPR_LITERAL_BOOL;
	b->value = value;
	return b;
}

VariableExpr* make_variable_expr(const char* name)
{
	VariableExpr* v = malloc(sizeof(VariableExpr));
	v->base.type = EXPR_VARIABLE;
	v->name = strdup(name);
	return v;
}

UnaryExpr* make_unary_expr(enum TokenType operator, Expr* operand)
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

AssignExpr* make_assign_expr(const char* name, Expr* value)
{
	AssignExpr* a = malloc(sizeof(AssignExpr));
	a->base.type = EXPR_ASSIGN;
	a->name = strdup(name);
	a->value = value;
	return a;
}

LogicalExpr* make_logical_expr(Expr* left, enum TokenType operator, Expr* right)
{
	LogicalExpr* l = malloc(sizeof(LogicalExpr));
	l->base.type = EXPR_LOGICAL;
	l->left = left;
	l->operator = operator;
	l->right = right;
	return l;
}

void free_expr(Expr* expr)
{
	if (!expr)
		return;

	switch (expr->type) {
		case EXPR_LITERAL_NUMBER:
		case EXPR_LITERAL_BOOL: {
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
		case EXPR_LOGICAL: {
			LogicalExpr* l = (LogicalExpr*)expr;
			free_expr(l->left);
			free_expr(l->right);
			free(l);
			break;
		}
		case EXPR_ASSIGN: {
			AssignExpr* a = (AssignExpr*)expr;
			free(a->name);
			free_expr(a->value);
			free(a);
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
