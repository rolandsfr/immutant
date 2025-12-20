#include "ast_expr.h"
#include "ast_make_expr.h"
#include "error.h"
#include "parse_and.h"
#include "parser_helpers.h"
#include "parser_singnature.h"

DEF_PARSE_FN(parse_or)
{
	Expr* expr = parse_and(tokens, pos, out_error);

	while (match_token(tokens, pos, 1, TOKEN_OR)) {
		Token operator = prev_token(tokens, *pos);

		Expr* right = parse_and(tokens, pos, out_error);

		if (out_error && out_error->type != ERROR_NONE || right == NULL) {
			return NULL;
		}

		expr =
			(Expr*)make_logical_expr(expr, operator.type, right, operator.line);
	}

	return expr;
}
