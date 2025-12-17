#include "parse_if_stmt.h"

#include "ast_cnstrct.h"
#include "ast_make_stmt.h"
#include "ast_stmt.h"
#include "error.h"
#include "lexer.h"
#include "parse_dclr.h"
#include "parse_expr.h"
#include "parser_helpers.h"

IfStmt* parse_if_stmt(TokenBuffer* tokens, size_t* pos, Error* out_error)
{
	if (!match_token(tokens, pos, 1, TOKEN_LEFT_PAREN)) {
		if (out_error) {
			*out_error = (Error){.type = SYNTAX_ERROR_UNEXPECTED_TOKEN,
								 .line = peek_token_full(tokens, *pos).line,
								 .message = "Expected '(' after 'if'"};
		}
		return NULL;
	}

	Expr* condition = parse_expr(tokens, pos, out_error);
	printf("DEBUG: parse_if_stmt: parsed condition\n");

	if (!condition) {
		if (out_error) {
			*out_error =
				(Error){.type = SYNTAX_ERROR_UNEXPECTED_TOKEN,
						.line = peek_token_full(tokens, *pos).line,
						.message = "Invalid expression in if condition"};
		} // TODO: should be handled by parse_primary (invalid logic there for
		  // fallback case in the for now...)
		return NULL;
	}

	if (!match_token(tokens, pos, 1, TOKEN_RIGHT_PAREN)) {
		if (out_error) {
			*out_error = (Error){.type = SYNTAX_ERROR_UNEXPECTED_TOKEN,
								 .line = peek_token_full(tokens, *pos).line,
								 .message = "Expected ')' after if condition"};
		}
		return NULL;
	}

	Stmt* then_branch = parse_dclr(tokens, pos, out_error);

	Stmt* else_branch = NULL;
	if (match_token(tokens, pos, 1, TOKEN_ELSE)) {
		else_branch = parse_dclr(tokens, pos, out_error);
	}

	return make_if_stmt(condition, then_branch, else_branch);
}
