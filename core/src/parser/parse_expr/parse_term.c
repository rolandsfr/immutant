#include "parse_term.h"

#include <stdio.h>

#include "ast_cnstrct.h"
#include "ast_make_expr.h"
#include "error_report.h"
#include "lexer.h"
#include "parse_factor.h"
#include "parse_lassoc.h"
#include "parser_helpers.h"
#include "parser_singnature.h"

DEF_PARSE_FN(parse_term)
{

	const TokenType operators[] = {TOKEN_MINUS, TOKEN_PLUS};
	const size_t operators_count = sizeof(operators) / sizeof(operators[0]);

	return parse_lassoc(tokens, pos, parse_factor, operators, operators_count,
						out_error);
}
