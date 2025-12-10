#include "parse_primary.h"

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

	if (match_token(tokens, pos, 1, TOKEN_UNRECOGNIZED)) {
		*out_error =
			make_error_report(ERROR_INVALID_TOKEN, "Invalid token encountered",
							  tokens->tokens[*pos].line);
		return NULL;
	}

	if (match_token(tokens, pos, 1, TOKEN_STRING)) {
		return (Expr*)make_string_expr(prev_token(tokens, *pos).lexeme);
	}

	if (match_token(tokens, pos, 1, TOKEN_NUMBER)) {
		return (Expr*)make_number_expr(prev_token(tokens, *pos).lexeme);
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
										   tokens->tokens[*pos].line);
			return NULL;
		}

		return expr;
	}

	*out_error = make_error_report(ERROR_UNEXPECTED_TOKEN,
								   "Expected expression after operator",
								   prev_token(tokens, *pos).line);

	return NULL;
}
