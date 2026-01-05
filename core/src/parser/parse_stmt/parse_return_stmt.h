#pragma once

#include <stddef.h>

typedef struct TokenBuffer TokenBuffer;
typedef struct Error Error;
typedef struct ReturnStmt ReturnStmt;

ReturnStmt* parse_return_stmt(TokenBuffer* tokens, size_t* pos,
							  Error* out_error);
