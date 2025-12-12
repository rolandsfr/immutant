#include "parse_expr.h"

#include <stdio.h>

#include "ast_cnstrct.h"
#include "ast_make_expr.h"
#include "lexer.h"
#include "parse_eq.h"

Expr* parse_expr(TokenBuffer* tokens, size_t* pos, Error* out_error)
{

	return parse_equality(tokens, pos, out_error);
}
