
#include "eval_binary.h"

#include <stdlib.h>
#include <string.h>

#include "ast_expr.h"
#include "error.h"
#include "eval_expr.h"
#include "eval_singature.h"
#include "is_equal.h"
#include "lexer.h"
#include "make_values.h"
#include "parse_asgn.h"
#include "parser_singnature.h"
#include "require_t.h"
#include "value_t.h"

DEF_EVAL_EXPR(eval_binary, BinaryExpr)
{
	Value left_value = eval_expr(expr->left, err, env);
	if (err->type != ERROR_NONE) {
		return make_null();
	}

	Value right_value = eval_expr(expr->right, err, env);
	if (err->type != ERROR_NONE) {
		return make_null();
	}

	switch (expr->operator) {
		case TOKEN_PLUS: {

			if (check_numbers(left_value, right_value, err, expr->base.line)) {
				return make_number(left_value.number + right_value.number);
			}

			if (check_strings(left_value, right_value, err, expr->base.line)) {
				size_t new_len =
					strlen(left_value.string) + strlen(right_value.string) + 1;
				char* concatenated = malloc(new_len);
				if (!concatenated) {
					if (err) {
						*err =
							(Error){.type = ERROR_OUT_OF_MEMORY,
									.message = "Memory allocation failed "
											   "during string concatenation"};
					}

					return make_null();
				}
				strcpy(concatenated, left_value.string);
				strcat(concatenated, right_value.string);
				return make_string(concatenated);
			}

			*err = (Error){
				.type = RUNTIME_UNEXPECTED_TYPE,
				.line = expr->base.line,
				.message =
					"Operands to '+' must be both numbers or both strings"};

			return make_null();
		}
		case TOKEN_MINUS: {
			if (require_numbers(left_value, right_value, err,
								expr->base.line)) {
				return make_number(left_value.number - right_value.number);
			}

			return make_null();
		}
		case TOKEN_STAR: {
			if (require_numbers(left_value, right_value, err,
								expr->base.line)) {
				return make_number(left_value.number * right_value.number);
			}
			return make_null();
		}
		case TOKEN_SLASH: {
			if (require_numbers(left_value, right_value, err,
								expr->base.line)) {
				if (right_value.number == 0) {
					if (err) {
						*err =
							(Error){.type = RUNTIME_DIVIDE_BY_ZERO,
									.line = expr->base.line,
									.message = "Division by zero is illegal"};
					}
					return make_null();
				}
				return make_number(left_value.number / right_value.number);
			}

			if (err) {
				*err = (Error){.type = RUNTIME_UNEXPECTED_TYPE,
							   .line = expr->base.line,
							   .message = "Operands to '/' must be numbers"};
			}
			return make_null();
		}
		case TOKEN_GREATER: {
			if (require_numbers(left_value, right_value, err,
								expr->base.line)) {
				return make_bool(left_value.number > right_value.number);
			}
			return make_null();
		}
		case TOKEN_GREATER_EQUAL: {
			if (require_numbers(left_value, right_value, err,
								expr->base.line)) {
				return make_bool(left_value.number >= right_value.number);
			}
			return make_null();
		}
		case TOKEN_LESS: {
			if (require_numbers(left_value, right_value, err,
								expr->base.line)) {
				return make_bool(left_value.number < right_value.number);
			}
			return make_null();
		}
		case TOKEN_LESS_EQUAL: {
			if (require_numbers(left_value, right_value, err,
								expr->base.line)) {
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

	fprintf(stderr, "Internal interpreter error: unknown binary operator\n");

	return make_null();
}
