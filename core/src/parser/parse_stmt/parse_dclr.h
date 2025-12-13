#pragma once

#include <stddef.h>

typedef struct TokenBuffer TokenBuffer;
typedef struct Error Error;
typedef struct Stmt Stmt;

Stmt* parse_dclr(TokenBuffer* tokens, size_t* pos, Error* out_error);
