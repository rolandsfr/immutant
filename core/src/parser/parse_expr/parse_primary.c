#include "parse_primary.h"

#include "ast_cnstrct.h"
#include "ast_make_expr.h"
#include "error.h"
#include "error_report.h"
#include "lexer.h"
#include "parse_eq.h"
#include "parse_lassoc.h"
#include "parser_helpers.h"
#include "parser_singnature.h"

DEF_PARSE_FN(parse_primary)
{
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

	if (match_token(tokens, pos, 1, TOKEN_IDENTIFIER)) {
		return (Expr*)make_variable_expr(prev_token(tokens, *pos).lexeme);
	}

	if (match_token(tokens, pos, 1, TOKEN_LEFT_PAREN)) {
		Expr* expr = parse_equality(tokens, pos, out_error);
		if (out_error && out_error->type != ERROR_NONE) {
			return NULL;
		}

		int is_matched = match_token(tokens, pos, 1, TOKEN_RIGHT_PAREN);

		if (!is_matched) {
			if (out_error) {
				*out_error = (Error){.type = SYNTAX_ERROR_UNEXPECTED_TOKEN,
									 .message = "Expected ')' after expression",
									 .line = tokens->tokens[*pos].line};
			}
			return NULL;
		}

		return expr;
	}

	if (prev_token(tokens, *pos).type != TOKEN_LEFT_PAREN && out_error) {
		*out_error = (Error){.type = SYNTAX_ERROR_MISSING_EXPRESSION,
							 .message = "Expected expression",
							 .line = tokens->tokens[*pos - 1].line};
	}

	return NULL;
}
