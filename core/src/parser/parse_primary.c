

#include "parse_primary.h"

#include <stdio.h>

#include "ast_cnstrct.h"
#include "ast_make_expr.h"
#include "error_codes.h"
#include "error_report.h"
#include "lexer.h"
#include "parse_eq.h"
#include "parse_lassoc.h"
#include "parser_helpers.h"
#include "parser_singnature.h"

DEF_PARSE_FN(parse_primary)
{

	Token current_token = tokens->tokens[*pos];

	if (match_token(tokens, pos, 1, TOKEN_STRING)) {
		return (Expr*)make_string_expr(current_token.lexeme);
	}

	if (match_token(tokens, pos, 1, TOKEN_NUMBER)) {
		return (Expr*)make_number_expr(current_token.lexeme);
	}

	if (match_token(tokens, pos, 1, TOKEN_TRUE)) {
		return (Expr*)make_boolean_expr(1);
	}

	if (match_token(tokens, pos, 1, TOKEN_FALSE)) {
		return (Expr*)make_boolean_expr(0);
	}

	if (match_token(tokens, pos, 1, TOKEN_LEFT_PAREN)) {
		Expr* expr = parse_equality(tokens, pos, out_error);
		if (out_error->code != NO_ERROR) {
			return NULL;
		}

		int is_matched = match_token(tokens, pos, 1, TOKEN_RIGHT_PAREN);

		if (!is_matched) {
			*out_error = make_error_report(ERROR_UNEXPECTED_TOKEN,
										   "Expected ')' after expression",
										   current_token.line);
			return NULL;
		}

		return expr;
	}

	return parse_equality(tokens, pos, out_error);
}
