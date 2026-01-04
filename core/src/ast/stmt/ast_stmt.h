#pragma once

#include <stddef.h>

#include "array.h"
#include "stdio.h"

#include "../ast_cnstrct.h"

enum MutabilityType { MUTABILITY_UNSET = -1, MUTABLE, IMMUTABLE };

enum PurityType { PURITY_UNSET = -1, PURE, IMPURE };

ARR_DEFINE(char*, ParamArray);

typedef struct VarDeclStmt {
	Stmt base;
	char* name;
	Expr* initializer;
	enum MutabilityType mutability;
} VarDeclStmt;

typedef struct FunDeclStmt {
	Stmt base;
	char* name;
	ParamArray* params;
	struct BlockStmt* body;
	enum PurityType purity;
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
