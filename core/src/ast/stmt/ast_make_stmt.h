#pragma once

#include "ast_expr.h"
#include "ast_stmt.h"

#include "../ast_cnstrct.h"

VarDeclStmt* make_var_decl_stmt(const char* name, Expr* initializer,
								enum MutabilityType mutability);

FunDeclStmt* make_fun_decl_stmt(const char* name, char** param_names,
								size_t param_count, Stmt* body);

ReturnStmt* make_return_stmt(Expr* value);

BlockStmt* make_block_stmt(Stmt** statements, size_t count);

IfStmt* make_if_stmt(Expr* condition, Stmt* then_branch, Stmt* else_branch);

ExprStmt* make_expr_stmt(Expr* expression);

WhileStmt* make_while_stmt(Expr* condition, Stmt* body);

void free_expr(Expr* expr);
