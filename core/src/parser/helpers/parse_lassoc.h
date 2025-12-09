#pragma once

#include <stddef.h>

#include "error_codes.h"
#include "error_report.h"
#include "parser_singnature.h"

typedef struct TokenBuffer TokenBuffer;
typedef struct Expr Expr;
typedef enum TokenType TokenType;

Expr* parse_lassoc(TokenBuffer* tokens, size_t* pos, ParseFn next_precedence_fn,
				   const enum TokenType* operators, size_t operator_count,
				   ErrorReport* out_error);
