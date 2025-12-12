#pragma once

#include <stddef.h>

typedef struct ExprStmt ExprStmt;
typedef struct TokenBuffer TokenBuffer;
typedef struct Error Error;

ExprStmt* parse_expr_stmt(TokenBuffer* tokens, size_t* pos, Error* out_error);
