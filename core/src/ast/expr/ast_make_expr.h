#include "ast_expr.h"

#include "../ast_cnstrct.h"

NumberExpr* make_number_expr(double value);

BinaryExpr* make_binary_expr(Expr* left, char op, Expr* right);

VariableExpr* make_variable_expr(const char* name);

UnaryExpr* make_unary_expr(char operator, Expr * operand);

CallExpr* make_call_expr(const char* name, Expr** args, size_t arg_count);
