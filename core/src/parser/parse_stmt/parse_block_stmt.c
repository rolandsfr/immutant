
#include "parse_block_stmt.h"

#include "ast_make_stmt.h"
#include "ast_stmt.h"
#include "error.h"
#include "lexer.h"
#include "parse_dclr.h"
#include "parser_helpers.h"

BlockStmt* parse_block_stmt(TokenBuffer* tokens, size_t* pos, Error* out_error)
{
	StmtBuffer stmt_buffer;
	StmtBuffer_init(&stmt_buffer);

	while (!check_token(tokens, *pos, TOKEN_RIGHT_BRACE) &&
		   !is_at_end(tokens, *pos)) {

		Stmt* stmt = parse_dclr(tokens, pos, out_error);

		if ((out_error && out_error->type != ERROR_NONE)) {
			StmtBuffer_free(&stmt_buffer);
			return NULL;
		}

		StmtBuffer_push(&stmt_buffer, stmt);
	}

	Token block_token = peek_token_full(tokens, *pos - 1);
	if (!match_token(tokens, pos, 1, TOKEN_RIGHT_BRACE)) {
		if (out_error) {
			*out_error = (Error){.type = SYNTAX_ERROR_UNEXPECTED_TOKEN,
								 .line = peek_token_full(tokens, *pos).line,
								 .message = ""};
			snprintf(out_error->message, sizeof(out_error->message),
					 "Expected '}' after block statement");
		}

		StmtBuffer_free(&stmt_buffer);
		return NULL;
	}

	return make_block_stmt(stmt_buffer.data, stmt_buffer.len, block_token.line);
}
