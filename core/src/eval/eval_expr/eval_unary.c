#include "eval_unary.h"

#include "ast_cnstrct.h"
#include "ast_expr.h"
#include "eval.h"
#include "make_runtime_err.h"
#include "make_values.h"
#include "require_t.h"
#include "runtime_err.h"
#include "value_t.h"

Value eval_unary(struct UnaryExpr* expr, RuntimeError* err)
{
	Value operand_value = eval_expr(expr->operand, err);

	if (err->type != RUNTIME_NO_ERROR) {
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
			*err = make_runtime_error(RUNTIME_INTERNAL_ERROR,
									  "Unknown unary operator");
			return make_null();
	}
}
