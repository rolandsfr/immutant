#include "ast_free.h"

#include <stdlib.h>

#include "ast_cnstrct.h"

#include "./expr/ast_expr.h"
#include "./stmt/ast_stmt.h"

void free_expr(Expr* expr)
{
	if (!expr)
		return;

	switch (expr->type) {
		case EXPR_LITERAL_NUMBER: {
			NumberExpr* n = (NumberExpr*)expr;
			free(n->value);
			free(n);
			break;
		}
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
