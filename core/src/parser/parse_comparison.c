#include "parse_comparison.h"

#include <stdio.h>

#include "ast_cnstrct.h"
#include "ast_make_expr.h"
#include "lexer.h"
#include "parse_lassoc.h"
#include "parse_term.h"
#include "parser_helpers.h"

Expr* parse_comparison(TokenBuffer* tokens, size_t* pos)
{

	const TokenType operators[] = {TOKEN_GREATER_EQUAL, TOKEN_GREATER,
								   TOKEN_LESS_EQUAL, TOKEN_LESS};
	const size_t operators_count = sizeof(operators) / sizeof(operators[0]);

	return parse_lassoc(tokens, pos, parse_term, operators, operators_count);
}
