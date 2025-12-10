
#include "eval_binary.h"

#include <stdlib.h>
#include <string.h>

#include "ast_expr.h"
#include "eval.h"
#include "eval_singature.h"
#include "is_equal.h"
#include "lexer.h"
#include "make_runtime_err.h"
#include "make_values.h"
#include "require_t.h"
#include "runtime_err.h"
#include "value_t.h"

DEF_EVAL_EXPR(eval_binary, BinaryExpr)
{
	Value left_value = eval_expr(expr->left, err);
	if (err->type != RUNTIME_NO_ERROR) {
		return make_null();
	}

	Value right_value = eval_expr(expr->right, err);
	if (err->type != RUNTIME_NO_ERROR) {
		return make_null();
	}

	switch (expr->operator) {
		case TOKEN_PLUS: {

			if (require_number(left_value, err) &&
				require_number(right_value, err)) {
				return make_number(left_value.number + right_value.number);
			}
			if (require_string(left_value, err) &&
				require_string(right_value, err)) {
				size_t new_len =
					strlen(left_value.string) + strlen(right_value.string) + 1;
				char* concatenated = malloc(new_len);
				if (!concatenated) {
					*err = make_runtime_error(
						RUNTIME_INTERNAL_ERROR,
						"Memory allocation failed during string concatenation");
					return make_null();
				}
				strcpy(concatenated, left_value.string);
				strcat(concatenated, right_value.string);
				return make_string(concatenated);
			}

			*err = make_runtime_error(
				RUNTIME_EXPECTED_DIFFERENT_TYPE,
				"Operands to '+' must be both numbers or both strings");

			return make_null();
		}
		case TOKEN_MINUS: {
			if (require_numbers(left_value, right_value, err)) {
				return make_number(left_value.number - right_value.number);
			}

			return make_null();
		}
		case TOKEN_STAR: {
			if (require_numbers(left_value, right_value, err)) {
				return make_number(left_value.number * right_value.number);
			}
			return make_null();
		}
		case TOKEN_SLASH: {
			if (require_numbers(left_value, right_value, err)) {
				if (right_value.number == 0) {
					*err = make_runtime_error(RUNTIME_DIVIDE_BY_ZERO,
											  "Division by zero is illegal");
					return make_null();
				}
				return make_number(left_value.number / right_value.number);
			}

			*err = make_runtime_error(RUNTIME_EXPECTED_DIFFERENT_TYPE,
									  "Operands to '/' must be numbers");
			return make_null();
		}
		case TOKEN_GREATER: {
			if (require_numbers(left_value, right_value, err)) {
				return make_bool(left_value.number > right_value.number);
			}
			return make_null();
		}
		case TOKEN_GREATER_EQUAL: {
			if (require_numbers(left_value, right_value, err)) {
				return make_bool(left_value.number >= right_value.number);
			}
			return make_null();
		}
		case TOKEN_LESS: {
			if (require_numbers(left_value, right_value, err)) {
				return make_bool(left_value.number < right_value.number);
			}
			return make_null();
		}
		case TOKEN_LESS_EQUAL: {
			if (require_numbers(left_value, right_value, err)) {
				return make_bool(left_value.number <= right_value.number);
			}
			return make_null();
		}
		case TOKEN_EQUAL_EQUAL: {
			return make_bool(is_equal(left_value, right_value));
		}
		case TOKEN_BANG_EQUAL: {
			return make_bool(!is_equal(left_value, right_value));
		}
	}

	*err =
		make_runtime_error(RUNTIME_INTERNAL_ERROR, "Unknown binary operator");
	return make_null();
}
