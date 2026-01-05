#include "parse_while_stmt.h"

#include "ast_make_stmt.h"
#include "ast_stmt.h"
#include "lexer.h"
#include "parse_dclr.h"
#include "parse_expr.h"
#include "parser_helpers.h"

WhileStmt* parse_while_stmt(TokenBuffer* tokens, size_t* pos, Error* out_error)
{

	if (!match_token(tokens, pos, 1, TOKEN_LEFT_PAREN)) {
		if (out_error) {
			*out_error = (Error){.type = SYNTAX_ERROR_UNEXPECTED_TOKEN,
								 .line = peek_token_full(tokens, *pos).line,
								 .message = "Expected '(' after 'while'"};
		}
		return NULL;
	}

	Expr* condition = parse_expr(tokens, pos, out_error);

	if (!condition) {
		if (out_error) {
			*out_error =
				(Error){.type = SYNTAX_ERROR_UNEXPECTED_TOKEN,
						.line = peek_token_full(tokens, *pos).line,
						.message = "Invalid expression in while condition"};
		} // TODO: should be handled by parse_primary (invalid logic there for
		  // fallback case in the for now...)
		return NULL;
	}

	if (!match_token(tokens, pos, 1, TOKEN_RIGHT_PAREN)) {
		if (out_error) {
			*out_error = (Error){.type = SYNTAX_ERROR_UNEXPECTED_TOKEN,
								 .line = peek_token_full(tokens, *pos).line,
								 .message = "Expected ')' after expression"};
		}
		return NULL;
	}

	Stmt* body_stmt = parse_dclr(tokens, pos, out_error);

	return make_while_stmt(condition, body_stmt, condition->line);
}
