#include "parse_comparison.h"

#include <stdio.h>

#include "ast_cnstrct.h"
#include "ast_make_expr.h"
#include "error_report.h"
#include "lexer.h"
#include "parse_lassoc.h"
#include "parse_term.h"
#include "parser_helpers.h"
#include "parser_singnature.h"

DEF_PARSE_FN(parse_comparison)
{

	const TokenType operators[] = {TOKEN_GREATER_EQUAL, TOKEN_GREATER,
								   TOKEN_LESS_EQUAL, TOKEN_LESS};
	const size_t operators_count = sizeof(operators) / sizeof(operators[0]);

	return parse_lassoc(tokens, pos, parse_term, operators, operators_count,
						out_error);
}
