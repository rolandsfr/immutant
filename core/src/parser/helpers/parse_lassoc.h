#pragma once

#include <stddef.h>

typedef struct TokenBuffer TokenBuffer;
typedef struct Expr Expr;
typedef enum TokenType TokenType;

typedef Expr* (*ParseFn)(TokenBuffer*, size_t*);

Expr* parse_lassoc(TokenBuffer* tokens, size_t* pos, ParseFn next_precedence_fn,
				   const enum TokenType* operators, size_t operator_count);
