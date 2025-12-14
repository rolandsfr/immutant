#include <stdio.h>
#include <stdlib.h>

#include "array.h"

typedef struct TokenBuffer TokenBuffer;
typedef struct ExprStmt ExprStmt;
typedef struct ErrorBuffer ErrorBuffer;

ARR_DEFINE(ExprStmt*, Stmts);

Stmts parse(TokenBuffer* tokens, ErrorBuffer* out_error);
