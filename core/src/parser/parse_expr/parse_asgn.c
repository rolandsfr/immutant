#include "parse_asgn.h"

#include "ast_make_expr.h"
#include "error.h"
#include "error_report.h"
#include "lexer.h"
#include "parse_or.h"
#include "parser_helpers.h"
#include "parser_singnature.h"

#include "ast/ast_cnstrct.h"

DEF_PARSE_FN(parse_asgn)
{
	Expr* expr = parse_or(tokens, pos, out_error);

	if (out_error && out_error->type != ERROR_NONE) {
		return NULL;
	}

	// additional top-level rule for assigment that can be skipped
	if (match_token(tokens, pos, 1, TOKEN_EQUAL)) {
		Token target = tokens->tokens[*pos - 2];
		// recursive call itself to allow chained assignments
		Expr* val_expr = parse_asgn(tokens, pos, out_error);
		if (out_error && out_error->type != ERROR_NONE) {
			free_expr(expr);
			return NULL;
		}

		if (expr->type != EXPR_VARIABLE) {
			if (out_error) {
				*out_error = (Error){
					.type = SYNTAX_ERROR_INVALID_ASSIGNMENT_TARGET,
					.line = target.line,
					.message = "Invalid assignment target.",
				};
			}
			free_expr(val_expr);
			free_expr(expr);
			return NULL;
		}

		return (Expr*)make_assign_expr(target.lexeme, val_expr, target.line);
	}

	return expr;
}
