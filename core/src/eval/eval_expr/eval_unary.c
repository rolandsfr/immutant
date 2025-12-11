#include "eval_unary.h"

#include "ast_cnstrct.h"
#include "ast_expr.h"
#include "error.h"
#include "eval.h"
#include "eval_singature.h"
#include "make_values.h"
#include "require_t.h"
#include "value_t.h"

DEF_EVAL_EXPR(eval_unary, UnaryExpr)
{
	Value operand_value = eval_expr(expr->operand, err);

	if (err && err->type != ERROR_NONE) {
		return make_null();
	}

	switch (expr->operator) {
		case TOKEN_MINUS: {
			double num = 0.0;

			if (!require_number(operand_value, err)) {
				return make_null();
			}

			num = -operand_value.number;

			return make_number(num);
		}
		case TOKEN_BANG: {
			int bool_val = 0;

			if (!require_bool(operand_value, err)) {
				return make_null();
			}

			bool_val = !operand_value.boolean;

			return make_bool(bool_val);
		}
		default:
			fprintf(stderr,
					"Internal interpreter error: unknown unary operator\n");
			return make_null();
	}
}
