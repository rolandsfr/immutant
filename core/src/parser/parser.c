#include "parser.h"

#include <stdlib.h>

#include "ast_stmt.h"
#include "error.h"
#include "lexer.h"
#include "parse_dclr.h"
#include "parse_expr_stmt.h"

void init_stmts_buffer(Stmts* stmts)
{
	stmts->count = 0;
	stmts->capacity = 8;
	stmts->expr_stmts = malloc(stmts->capacity * sizeof(ExprStmt*));
}

void add_stmt(Stmts* stmts, Stmt* expr_stmt)
{
	if (stmts->count >= stmts->capacity) {
		stmts->capacity *= 2;
		stmts->expr_stmts =
			realloc(stmts->expr_stmts, stmts->capacity * sizeof(ExprStmt*));
	}
	stmts->expr_stmts[stmts->count++] = expr_stmt;
}

Stmts parse(TokenBuffer* tokens, Error* out_error)
{
	size_t pos = 0;

	Stmts stmts;
	init_stmts_buffer(&stmts);

	while (!is_at_end(tokens, pos)) {
		Stmt* stmt = parse_dclr(tokens, &pos, out_error);

		if (out_error && out_error->type != ERROR_NONE || stmt == NULL) {
			break;
		}

		add_stmt(&stmts, stmt);
	}

	return stmts;
}
