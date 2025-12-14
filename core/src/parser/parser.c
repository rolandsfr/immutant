#include "parser.h"

#include <stdlib.h>

#include "ast_stmt.h"
#include "error.h"
#include "lexer.h"
#include "parse_dclr.h"
#include "parse_expr_stmt.h"
#include "sync_parse.h"

Stmts parse(TokenBuffer* tokens, ErrorBuffer* out_errors)
{
	size_t pos = 0;

	Stmts stmts;
	Stmts_init(&stmts);

	while (!is_at_end(tokens, pos)) {
		Error error = {-1};
		Stmt* stmt = parse_dclr(tokens, &pos, &error);

		if (error.type != ERROR_NONE || stmt == NULL) {
			ErrorBuffer_push(out_errors, error);
			sync_parse(tokens, &pos, &error);
			continue;
		}

		Stmts_push(&stmts, (ExprStmt*)stmt);
	}

	return stmts;
}
