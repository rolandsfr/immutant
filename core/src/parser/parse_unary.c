
#include "parse_unary.h"

#include <stdio.h>

#include "ast_cnstrct.h"
#include "ast_make_expr.h"
#include "lexer.h"
#include "parse_lassoc.h"
#include "parse_primary.h"
#include "parser_helpers.h"

Expr* parse_unary(TokenBuffer* tokens, size_t* pos, ErrorCode* out_error)
{
	if (match_token(tokens, pos, 1, TOKEN_MINUS, TOKEN_BANG)) {
		TokenType operator_token = previous_token(tokens, *pos);
		Expr* right = parse_unary(tokens, pos, out_error);
		return (Expr*)make_unary_expr(operator_token, right);
	}

	return parse_primary(tokens, pos, out_error);
}
