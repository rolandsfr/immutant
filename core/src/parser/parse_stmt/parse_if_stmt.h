#pragma once

#include <stddef.h>

typedef struct TokenBuffer TokenBuffer;
typedef struct Error Error;
typedef struct IfStmt IfStmt;

IfStmt* parse_if_stmt(TokenBuffer* tokens, size_t* pos, Error* out_error);
