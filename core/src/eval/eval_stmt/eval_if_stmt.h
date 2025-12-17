#pragma once

typedef struct IfStmt IfStmt;
typedef struct Error Error;
typedef struct Env Env;
typedef struct Value Value;

void eval_if_stmt(IfStmt* stmt, Error* err, Env* env, Value* out_value);
