#pragma once

#include <stddef.h>

#include "ast_expr.h"
#include "error.h"
#include "lexer.h"

Expr* parse_expr(TokenBuffer* tokens, size_t* pos, Error* out_error);
