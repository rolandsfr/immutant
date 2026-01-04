#pragma once

typedef struct FunDeclStmt FunDeclStmt;
typedef struct Env Env;
typedef struct Error Error;

void def_user_fn(FunDeclStmt* declaration, Env* env, Error* err);
