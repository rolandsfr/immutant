#pragma once

typedef struct Stmt Stmt;
typedef struct Error Error;
typedef struct Value Value;
typedef struct Env Env;

void eval(Stmt* stmt, Error* err, Value* out_value, Env* env);
