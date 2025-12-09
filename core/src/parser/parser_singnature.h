#pragma once

#include <stddef.h>
typedef struct TokenBuffer TokenBuffer;
typedef struct ErrorReport ErrorReport;
typedef struct Expr Expr;

#define DEF_PARSE_FN(name)                                                     \
	Expr* name(TokenBuffer* tokens, size_t* pos, ErrorReport* out_error)

typedef DEF_PARSE_FN(ParseFn);
