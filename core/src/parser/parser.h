#include <stdlib.h>

typedef struct TokenBuffer TokenBuffer;
typedef struct ExprStmt ExprStmt;
typedef struct ErrorBuffer ErrorBuffer;

typedef struct Stmts {
	ExprStmt** expr_stmts;
	size_t count;
	size_t capacity;
} Stmts;

void init_stmts_buffer(Stmts* stmts);

void add_expr_stmt(Stmts* stmts, ExprStmt* expr_stmt);

Stmts parse(TokenBuffer* tokens, ErrorBuffer* out_error);
