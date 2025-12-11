#include "parse_lassoc.h"

#include "ast_cnstrct.h"
#include "ast_expr.h"
#include "ast_make_expr.h"
#include "error.h"
#include "lexer.h"
#include "parser_helpers.h"
#include "parser_singnature.h"

Expr* parse_lassoc(TokenBuffer* tokens, size_t* pos, ParseFn next_precedence_fn,
				   const enum TokenType* operators, size_t operator_count,
				   Error* out_error)
{
	Expr* expr = next_precedence_fn(tokens, pos, out_error);

	if (out_error && out_error->type != ERROR_NONE) {
		return NULL;
	}

	while (match_any_token(tokens, pos, operators, operator_count)) {
		Token operator = tokens->tokens[*pos - 1];
		Expr* right = next_precedence_fn(tokens, pos, out_error);

		if (out_error && out_error->type != ERROR_NONE || right == NULL) {
			return NULL;
		}

		// creates nested binary expressions for left-associative chains
		// left and right are sub-expressions that can be further binary
		// expressions or literals/variables
		expr = (Expr*)make_binary_expr(expr, operator.type, right);
	}

	return expr;
}
