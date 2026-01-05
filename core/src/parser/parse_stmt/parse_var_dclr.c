#include "ast_make_expr.h"
#include "ast_make_stmt.h"
#include "ast_stmt.h"
#include "error.h"
#include "lexer.h"
#include "parse_expr.h"
#include "parser_helpers.h"

VarDeclStmt* parse_var_decl(TokenBuffer* tokens, size_t* pos,
							enum MutabilityType mutability, Error* out_error)
{

	Token name = consume_token(tokens, pos);
	if (name.type != TOKEN_IDENTIFIER) {
		if (out_error) {
			*out_error = (Error){.type = SYNTAX_ERROR_UNEXPECTED_TOKEN,
								 .line = name.line,
								 .message = "Expected variable name"};
		}
		return NULL;
	}
	Expr* initializer = NULL;

	if (match_token(tokens, pos, 1, TOKEN_EQUAL)) {
		initializer = parse_expr(tokens, pos, out_error);
		if (out_error && out_error->type != ERROR_NONE) {
			return NULL;
		}
	} else if (mutability == IMMUTABLE) {
		if (out_error) {
			*out_error =
				(Error){.type = SYNTAX_ERROR_UNEXPECTED_TOKEN,
						.message = "Immutable variables must be initialized.",
						.line = name.line};
		}
		return NULL;
	}

	if (!match_token(tokens, pos, 1, TOKEN_SEMICOLON)) {
		if (out_error) {
			*out_error =
				(Error){.type = SYNTAX_ERROR_UNEXPECTED_TOKEN,
						.message = "Expected ';' after variable declaration.",
						.line = peek_token_full(tokens, *pos).line};
		}

		free_expr(initializer);
		return NULL;
	}

	return make_var_decl_stmt(name.lexeme, initializer, mutability, name.line);
}
