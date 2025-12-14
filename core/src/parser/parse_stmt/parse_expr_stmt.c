#include "parse_expr_stmt.h"

#include "ast_cnstrct.h"
#include "ast_make_stmt.h"
#include "ast_stmt.h"
#include "error.h"
#include "parse_expr.h"
#include "parser_helpers.h"

ExprStmt* parse_expr_stmt(TokenBuffer* tokens, size_t* pos, Error* out_error)
{
	Expr* expr = parse_expr(tokens, pos, out_error);
	if (out_error && out_error->type != ERROR_NONE || expr == NULL) {
		return NULL;
	}

	if (!match_token(tokens, pos, 1, TOKEN_SEMICOLON)) {
		if (out_error) {

			Token token = peek_token_full(tokens, *pos);
			*out_error =
				(Error){.type = SYNTAX_ERROR_UNEXPECTED_TOKEN,
						.message = "Expected ';' after expression statement.",
						.line = token.line};
		}

		free_expr(expr);

		return NULL;
	}

	return make_expr_stmt(expr);
}
