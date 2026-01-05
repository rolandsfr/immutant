
#include "parse_return_stmt.h"

#include "ast_make_stmt.h"
#include "ast_stmt.h"
#include "lexer.h"
#include "parse_expr.h"
#include "parser_helpers.h"

ReturnStmt* parse_return_stmt(TokenBuffer* tokens, size_t* pos,
							  Error* out_error)
{

	Expr* value = NULL;

	// if the value exsists (return stmt can have no value), parse it
	// aka if there is ANYTHING other than a semicolon after return (other than
	// end of statement)

	if (!match_token(tokens, pos, 1, TOKEN_SEMICOLON)) {
		value = parse_expr(tokens, pos, out_error);

		if (out_error && out_error->type != ERROR_NONE) {
			return NULL;
		}
	}

	if (!match_token(tokens, pos, 1, TOKEN_SEMICOLON)) {
		if (out_error) {
			*out_error =
				(Error){.type = SYNTAX_ERROR_UNEXPECTED_TOKEN,
						.line = peek_token_full(tokens, *pos).line,
						.message = "Expected ';' after return statement"};
		}

		free_expr(value);
		return NULL;
	}

	Token return_token = peek_token_full(tokens, *pos - 1);
	return make_return_stmt(value, return_token.line);
}
