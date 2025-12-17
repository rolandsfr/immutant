#pragma once

#include <stdio.h>
#include <stdlib.h>

#include "array.h"
#include "ast_cnstrct.h"

typedef struct TokenBuffer TokenBuffer;
typedef struct ExprStmt ExprStmt;
typedef struct ErrorBuffer ErrorBuffer;

ARR_DEFINE(Stmt*, Stmts);

Stmts parse(TokenBuffer* tokens, ErrorBuffer* out_error);
