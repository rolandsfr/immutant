#pragma once

typedef struct Expr Expr;
typedef struct Value Value;
typedef struct RuntimeError RuntimeError;

Value eval_expr(Expr* expr, RuntimeError* err);
