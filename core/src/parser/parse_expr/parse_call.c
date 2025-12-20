#include "parse_call.h"

#include "ast_cnstrct.h"
#include "ast_expr.h"
#include "ast_make_expr.h"
#include "error.h"
#include "parse_expr.h"
#include "parse_primary.h"
#include "parser_helpers.h"
#include "parser_singnature.h"

DEF_PARSE_FN(parse_call)
{
	Expr* expr = parse_primary(tokens, pos, out_error);

	if (out_error && out_error->type != ERROR_NONE) {
		return NULL;
	}

	while (1) {
		if (match_token(tokens, pos, 1, TOKEN_LEFT_PAREN)) {
			ArgumentsArray arguments;
			ArgumentsArray_init(&arguments);

			if (!check_token(tokens, *pos, TOKEN_RIGHT_PAREN)) {
				do {
					Expr* argument = parse_expr(tokens, pos, out_error);
					if (out_error && out_error->type != ERROR_NONE) {
						// free previously parsed arguments
						for (size_t i = 0; i < arguments.len; i++) {
							free_expr(arguments.data[i]);
						}
						ArgumentsArray_free(&arguments);
						return NULL;
					}
					ArgumentsArray_push(&arguments, argument);
				} while (match_token(tokens, pos, 1, TOKEN_COMMA) &&
						 !is_at_end(tokens, *pos));
			}

			Token paren = consume_token(tokens, pos);

			if (paren.type != TOKEN_RIGHT_PAREN) {
				if (out_error) {
					*out_error =
						(Error){.type = SYNTAX_ERROR_UNEXPECTED_TOKEN,
								.line = paren.line,
								.message = "Expect ')' after arguments."};
				}

				for (size_t i = 0; i < arguments.len; i++) {
					free_expr(arguments.data[i]);
				}

				ArgumentsArray_free(&arguments);
				return NULL;
			}

			// parser should not restrict to only variable names as callees
			return (Expr*)make_call_expr(expr, arguments.data, arguments.len,
										 paren.line);
		}

		return expr;
	}
}
