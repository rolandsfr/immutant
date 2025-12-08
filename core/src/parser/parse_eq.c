
#include "parse_eq.h"

#include <stdio.h>

#include "ast_cnstrct.h"
#include "ast_make_expr.h"
#include "lexer.h"
#include "parse_comparison.h"
#include "parse_lassoc.h"
#include "parser_helpers.h"

Expr* parse_equality(TokenBuffer* tokens, size_t* pos)
{

	const TokenType operators[] = {TOKEN_BANG_EQUAL, TOKEN_EQUAL_EQUAL};
	const size_t operators_count = sizeof(operators) / sizeof(operators[0]);

	return parse_lassoc(tokens, pos, parse_comparison, operators,
						operators_count);
}
