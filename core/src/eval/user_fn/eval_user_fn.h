#pragma once

#include <stddef.h>

typedef struct Env Env;
typedef struct Value Value;
typedef struct FunDeclStmt FunDeclStmt;

Value create_user_fn(Env* env, FunDeclStmt* declaration);
