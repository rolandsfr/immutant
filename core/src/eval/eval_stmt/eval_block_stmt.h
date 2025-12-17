#pragma once

typedef struct BlockStmt BlockStmt;
typedef struct Error Error;
typedef struct Env Env;
typedef struct Value Value;

void eval_block_stmt(BlockStmt* stmt, Error* err, Env* env, Value* out_value);
