
#pragma once

typedef struct Expr Expr;
typedef struct Value Value;
typedef struct Error Error;
typedef struct Env Env;

Value eval_expr(Expr* expr, Error* err, Env* env);
