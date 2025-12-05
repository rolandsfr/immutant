
#include <stdlib.h>
#include <string.h>

#include "ast_expr.h"

#include "../ast_cnstrct.h"

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

BinaryExpr* make_binary_expr(Expr* left, char operator, Expr* right)
{
	BinaryExpr* b = malloc(sizeof(BinaryExpr));
	b->base.type = EXPR_BINARY;
	b->left = left;
	b->operator = operator;
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
