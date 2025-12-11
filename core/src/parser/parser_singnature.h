#pragma once

#include <stddef.h>
typedef struct TokenBuffer TokenBuffer;
typedef struct Expr Expr;
typedef struct Error Error;

#define DEF_PARSE_FN(name)                                                     \
	Expr* name(TokenBuffer* tokens, size_t* pos, Error* out_error)

typedef DEF_PARSE_FN(ParseFn);
