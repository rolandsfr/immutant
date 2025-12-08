#include "ast_expr.h"
#include "lexer.h"

#include "../ast_cnstrct.h"

// literal expression factories
StringExpr* make_string_expr(const char* value);

// NumberExpr* make_number_expr(double value);
NumberExpr* make_number_expr(const char* value);

BooleanExpr* make_boolean_expr(int value);

// more complex expression factories
BinaryExpr* make_binary_expr(Expr* left, enum TokenType op, Expr* right);

VariableExpr* make_variable_expr(const char* name);

UnaryExpr* make_unary_expr(enum TokenType operator, Expr * operand);

CallExpr* make_call_expr(const char* name, Expr** args, size_t arg_count);
