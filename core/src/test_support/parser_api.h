#pragma once

// Declarations provided by libparser.a. Parser integration tests must include
// only this header (not individual parse_*.h files) so Ceedling does not also
// compile the recursive-descent sources.

#include "array.h"
#include "ast_cnstrct.h"
#include "ast_expr.h"
#include "ast_stmt.h"
#include "error.h"
#include "parser_singnature.h"

typedef struct ErrorBuffer ErrorBuffer;

ARR_DEFINE(Stmt*, Stmts);

Expr* parse_expr(TokenBuffer* tokens, size_t* pos, Error* out_error);
Stmt* parse_dclr(TokenBuffer* tokens, size_t* pos, Error* out_error);
Stmts parse(TokenBuffer* tokens, ErrorBuffer* out_errors);

void free_expr(Expr* expr);
