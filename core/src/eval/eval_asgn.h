#pragma once

typedef struct AssignExpr AssignExpr;
typedef struct Env Env;
typedef struct Error Error;
typedef struct Value Value;

Value eval_asgn(AssignExpr* expr, Env* env, Error* out_error);
