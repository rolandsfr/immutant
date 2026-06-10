#pragma once

#include <stddef.h>

#include "ast_cnstrct.h"
#include "ast_stmt.h"
#include "error.h"
#include "lexer.h"

Stmt* parse_dclr(TokenBuffer* tokens, size_t* pos, Error* out_error);
