
#include "eval_logic.h"

#include <stdlib.h>
#include <string.h>

#include "ast_expr.h"
#include "error.h"
#include "eval_expr.h"
#include "eval_logic.h"
#include "eval_singature.h"
#include "is_equal.h"
#include "lexer.h"
#include "make_values.h"
#include "parse_asgn.h"
#include "parser_singnature.h"
#include "require_t.h"
#include "value_t.h"

DEF_EVAL_EXPR(eval_logic, LogicalExpr)
{
	Value left = eval_expr(expr->left, err, env);
	if (err && err->type != ERROR_NONE) {
		return make_null();
	}

	if (!require_bool(left, err)) {
		return make_null();
	}
	if (expr->operator == TOKEN_OR) {
		if (left.boolean) {
			return make_bool(1);
		}
	} else if (expr->operator == TOKEN_AND) {
		if (!left.boolean) {
			printf("Short-circuiting AND expression\n");
			return make_bool(0);
		}
	} else {
		if (err) {
			*err = (Error){.type = RUNTIME_UNEXPECTED_TYPE,
						   .message = "Invalid logical operator"};
		}
		return make_null();
	}

	Value right = eval_expr(expr->right, err, env);

	if (!require_bool(right, err)) {
		return make_null();
	}

	return make_bool(right.boolean);
}
