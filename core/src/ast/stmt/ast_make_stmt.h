#pragma once

#include "ast_expr.h"
#include "ast_stmt.h"

#include "../ast_cnstrct.h"

VarDeclStmt* make_var_decl_stmt(const char* name, Expr* initializer,
								enum MutabilityType mutability, int line_nr);
FunDeclStmt* make_fun_decl_stmt(const char* name, ParamArray* params,

								size_t param_count, BlockStmt* body,
								enum PurityType purity, int line_nr);
ReturnStmt* make_return_stmt(Expr* value, int line_nr);

BlockStmt* make_block_stmt(Stmt** statements, size_t count, int line_nr);

IfStmt* make_if_stmt(Expr* condition, Stmt* then_branch, Stmt* else_branch,
					 int line_nr);

ExprStmt* make_expr_stmt(Expr* expression, int line_nr);

WhileStmt* make_while_stmt(Expr* condition, Stmt* body, int line_nr);

void free_expr(Expr* expr);
