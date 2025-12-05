
#include "parse_eq.h"

#include "ast_cnstrct.h"
#include "ast_make_expr.h"
#include "lexer.h"
#include "parse_comparison.h"
#include "parser_helpers.h"

Expr* parse_equality(TokenBuffer* tokens, size_t* pos)
{
	Expr* expr = parse_comparison(tokens, pos);

	while (match_token(tokens, pos, 2, TOKEN_BANG_EQUAL, TOKEN_EQUAL_EQUAL)) {
		Token operator = tokens->tokens[*pos - 1];
		Expr* right = parse_comparison(tokens, pos);
		expr = (Expr*)make_binary_expr(expr, *operator.lexeme, right);
	}

	return expr;
}
