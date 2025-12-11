#pragma once

typedef struct Expr Expr;
typedef struct Value Value;
typedef struct Error Error;

Value eval_expr(Expr* expr, Error* err);
