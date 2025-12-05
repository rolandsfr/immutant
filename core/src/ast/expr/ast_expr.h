#pragma once

#include <stddef.h>

#include "../ast_cnstrct.h"

typedef struct NumberExpr {
	Expr base;
	double value;
} NumberExpr;

typedef struct StringExpr {
	Expr base;
	char* value;
} StringExpr;

typedef struct VariableExpr {
	Expr base;
	char* name;
} VariableExpr;

typedef struct UnaryExpr {
	Expr base;
	char operator;
	Expr* operand;
} UnaryExpr;

typedef struct BinaryExpr {
	Expr base;
	Expr* left;
	Expr* right;
	char operator;
} BinaryExpr;

typedef struct CallExpr {
	Expr base;
	char* name;
	Expr** args;
	size_t arg_count;
} CallExpr;
