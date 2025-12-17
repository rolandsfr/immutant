#include "parse_dclr.h"

#include "ast_stmt.h"
#include "error.h"
#include "lexer.h"
#include "parse_block_stmt.h"
#include "parse_expr_stmt.h"
#include "parse_var_dclr.h"
#include "parser_helpers.h"

Stmt* parse_dclr(TokenBuffer* tokens, size_t* pos, Error* out_error)
{
	if (match_token(tokens, pos, 2, TOKEN_MUTANT, TOKEN_IMMUTANT)) {
		enum MutabilityType mutability =
			previous_token(tokens, *pos) == TOKEN_MUTANT ? MUTABLE : IMMUTABLE;

		return (Stmt*)parse_var_decl(tokens, pos, mutability, out_error);
	}

	if (match_token(tokens, pos, 1, TOKEN_LEFT_BRACE)) {
		return (Stmt*)parse_block_stmt(tokens, pos, out_error);
	}

	return (Stmt*)parse_expr_stmt(tokens, pos, out_error);
}
