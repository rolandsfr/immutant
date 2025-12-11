
#include "parse_eq.h"

#include <stdio.h>

#include "ast_cnstrct.h"
#include "ast_make_expr.h"
#include "lexer.h"
#include "parse_comparison.h"
#include "parse_lassoc.h"
#include "parser_helpers.h"
#include "parser_singnature.h"

DEF_PARSE_FN(parse_equality)
{

	const TokenType operators[] = {TOKEN_BANG_EQUAL, TOKEN_EQUAL_EQUAL};
	const size_t operators_count = sizeof(operators) / sizeof(operators[0]);

	return parse_lassoc(tokens, pos, parse_comparison, operators,
						operators_count, out_error);
}
