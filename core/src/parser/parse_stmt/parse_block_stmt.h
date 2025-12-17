#pragma once

#include <stdio.h>

#include "array.h"
#include "ast_cnstrct.h"
#include "ast_stmt.h"

ARR_DEFINE(Stmt*, StmtBuffer);

typedef struct TokenBuffer TokenBuffer;
typedef struct Error Error;

BlockStmt* parse_block_stmt(TokenBuffer* tokens, size_t* pos, Error* out_error);
