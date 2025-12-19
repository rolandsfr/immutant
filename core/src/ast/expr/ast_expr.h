#pragma once

#include <stddef.h>

#include "lexer.h"

#include "../ast_cnstrct.h"

typedef struct NumberExpr {
	Expr base;
	// double value;
	char* value;
} NumberExpr;

typedef struct StringExpr {
	Expr base;
	char* value;
} StringExpr;

typedef struct BooleanExpr {
	Expr base;
	int value;
} BooleanExpr;

typedef struct VariableExpr {
	Expr base;
	char* name;
} VariableExpr;

typedef struct UnaryExpr {
	Expr base;
	enum TokenType operator;
	Expr* operand;
} UnaryExpr;

typedef struct BinaryExpr {
	Expr base;
	Expr* left;
	Expr* right;
	enum TokenType operator;
} BinaryExpr;

typedef struct AssignExpr {
	Expr base;
	char* name;
	Expr* value;
} AssignExpr;

typedef struct CallExpr {
	Expr base;
	Expr* callee;
	Expr** args;
	size_t arg_count;
} CallExpr;

typedef struct LogicalExpr {
	Expr base;
	Expr* left;
	Expr* right;
	enum TokenType operator;
} LogicalExpr;
