#pragma once

#include "ast_expr.h"
#include "lexer.h"

#include "../ast_cnstrct.h"

// literal expression factories
StringExpr* make_string_expr(const char* value, int line);

// NumberExpr* make_number_expr(double value);
NumberExpr* make_number_expr(const char* value, int line);

BooleanExpr* make_boolean_expr(int value, int line);

// more complex expression factories
BinaryExpr* make_binary_expr(Expr* left, enum TokenType op, Expr* right,
							 int line);

VariableExpr* make_variable_expr(const char* name, int line);

AssignExpr* make_assign_expr(const char* name, Expr* value, int line);

UnaryExpr* make_unary_expr(enum TokenType operator, Expr * operand, int line);

CallExpr* make_call_expr(Expr* callee, Expr** args, size_t arg_count, int line);

LogicalExpr* make_logical_expr(Expr* left, enum TokenType operator,
							   Expr * right, int line);

void free_expr(Expr* expr);
