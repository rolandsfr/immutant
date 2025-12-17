#pragma once

#include <stddef.h>

#include "../ast_cnstrct.h"

enum MutabilityType { MUTABLE, IMMUTABLE };

typedef struct VarDeclStmt {
	Stmt base;
	char* name;
	Expr* initializer;
	enum MutabilityType mutability;
} VarDeclStmt;

typedef struct FunDeclStmt {
	Stmt base;
	char* name;
	char** param_names;
	size_t param_count;
	Stmt* body; // block statement
} FunDeclStmt;

typedef struct ReturnStmt {
	Stmt base;
	Expr* value; // can be NULL
} ReturnStmt;

typedef struct BlockStmt {
	Stmt base;
	Stmt** statements;
	size_t count;
} BlockStmt;

typedef struct IfStmt {
	Stmt base;
	Expr* condition;
	Stmt* then_branch;
	Stmt* else_branch; // optional, can be NULL
} IfStmt;

typedef struct WhileStmt {
	Stmt base;
	Expr* condition;
	Stmt* body;
} WhileStmt;

typedef struct ExprStmt {
	Stmt base;
	Expr* expression;
} ExprStmt;
