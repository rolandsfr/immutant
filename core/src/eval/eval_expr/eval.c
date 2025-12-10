#include "eval.h"

#include <stdlib.h>
#include <string.h>

#include "ast_cnstrct.h"
#include "ast_expr.h"
#include "eval_binary.h"
#include "eval_unary.h"
#include "make_runtime_err.h"
#include "make_values.h"
#include "runtime_err.h"
#include "value_t.h"

Value eval_expr(Expr* expr, RuntimeError* err)
{
	switch (expr->type) {
		case EXPR_LITERAL_NUMBER: {
			NumberExpr* n = (NumberExpr*)expr;
			return make_number(strtod(n->value, NULL));
		}

		case EXPR_LITERAL_BOOL: {
			BooleanExpr* b = (BooleanExpr*)expr;
			return make_bool(b->value);
		}

		case EXPR_LITERAL_STRING: {
			StringExpr* s = (StringExpr*)expr;
			return make_string(strdup(s->value));
		}

			// case EXPR_VARIABLE:
			// 	return eval_variable((VariableExpr*)expr, err);

		case EXPR_UNARY:
			return eval_unary((UnaryExpr*)expr, err);

		case EXPR_BINARY:
			return eval_binary((BinaryExpr*)expr, err);

			// case EXPR_CALL:
			// 	return eval_call((CallExpr*)expr, err);
	}

	*err =
		make_runtime_error(RUNTIME_INTERNAL_ERROR, "Unknown expression type");
	return make_null();
}
