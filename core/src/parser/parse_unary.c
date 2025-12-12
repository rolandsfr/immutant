#include "parse_unary.h"

#include <stdio.h>

#include "ast_cnstrct.h"
#include "ast_make_expr.h"
#include "error.h"
#include "lexer.h"
#include "parse_lassoc.h"
#include "parse_primary.h"
#include "parser_helpers.h"
#include "parser_singnature.h"

DEF_PARSE_FN(parse_unary)
{
	if (match_token(tokens, pos, 2, TOKEN_MINUS, TOKEN_BANG)) {
		TokenType operator_token = previous_token(tokens, *pos);
		Expr* right = parse_unary(tokens, pos, out_error);

		if (out_error && out_error->type != ERROR_NONE || right == NULL) {
			return NULL;
		}

		return (Expr*)make_unary_expr(operator_token, right);
	}

	return parse_primary(tokens, pos, out_error);
}
