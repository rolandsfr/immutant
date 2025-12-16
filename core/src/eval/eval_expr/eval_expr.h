#pragma once

#include "value_t.h"

typedef struct Expr Expr;
typedef struct Error Error;
typedef struct Env Env;

Value eval_expr(Expr* expr, Error* err, Env* env);
