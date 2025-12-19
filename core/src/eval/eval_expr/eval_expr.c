#include "eval_expr.h"

#include <stdlib.h>
#include <string.h>

#include "ast_cnstrct.h"
#include "ast_expr.h"
#include "error.h"
#include "eval_asgn.h"
#include "eval_binary.h"
#include "eval_call.h"
#include "eval_logic.h"
#include "eval_unary.h"
#include "make_values.h"
#include "value_t.h"

#include "env/env.h"
#include "eval_vars/eval_var.h"

Value eval_expr(Expr* expr, Error* err, Env* env)
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

		case EXPR_UNARY:
			return eval_unary((UnaryExpr*)expr, err, env);

		case EXPR_BINARY:
			return eval_binary((BinaryExpr*)expr, err, env);

		case EXPR_LOGICAL:
			return eval_logic((LogicalExpr*)expr, err, env);

		case EXPR_CALL: {
			return eval_call((CallExpr*)expr, err, env);
		}

		case EXPR_VARIABLE: {
			VariableExpr* var_expr = (VariableExpr*)expr;
			Value value;
			eval_var_expr(env, var_expr, &value, err);
			return value;
		}

		case EXPR_ASSIGN: {
			AssignExpr* assign_expr = (AssignExpr*)expr;
			return eval_asgn(assign_expr, env, err);
		}
	}

	fprintf(stderr, "Internal interpreter error: unknown expression type\n");

	return make_null();
}
