
#pragma once

typedef struct WhileStmt WhileStmt;
typedef struct Error Error;
typedef struct Env Env;
typedef struct Value Value;

void eval_while_stmt(WhileStmt* stmt, Error* err, Env* env, Value* out_value);
