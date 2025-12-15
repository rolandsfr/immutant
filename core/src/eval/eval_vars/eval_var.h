#pragma once

typedef struct Env Env;
typedef struct VarDeclStmt VarDeclStmt;
typedef struct VariableExpr VariableExpr;
typedef struct Value Value;
typedef struct Error Error;

void eval_var_dclr(Env* env, VarDeclStmt* var_stmt, Error* error);

void eval_var_expr(Env* env, VariableExpr* var_expr, Value* out_value,
				   Error* error);
