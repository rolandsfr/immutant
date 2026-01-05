#pragma once

typedef struct ReturnStmt ReturnStmt;
typedef struct Error Error;
typedef struct Env Env;

void eval_return_stmt(ReturnStmt* stmt, Error* err, Env* env);
