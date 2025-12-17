#pragma once

#include <stddef.h>

typedef struct WhileStmt WhileStmt;
typedef struct TokenBuffer TokenBuffer;
typedef struct Error Error;

WhileStmt* parse_while_stmt(TokenBuffer* tokens, size_t* pos, Error* out_error);
