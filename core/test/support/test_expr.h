#pragma once

#include "ast_expr.h"
typedef struct Expr Expr;
typedef struct BinaryExpr BinaryExpr;
typedef struct VariableExpr VariableExpr;
typedef enum TokenType TokenType;

void TEST_ASSERT_UNARY_NUMBER_XPR(Expr* expr, enum TokenType expected_operator,
								  char* value);
void TEST_ASSERT_VAR_EXPR(Expr* expr, const char* expected_name);

void TEST_ASSERT_BINARY_EXPR(BinaryExpr* expr,
							 enum TokenType expected_operator);

void TEST_ASSERT_LITERAL_NUMBER_EXPR(Expr* expr, const char* expected_value);

void TEST_ASSERT_LITERAL_STRING_EXPR(Expr* expr, const char* expected_value);

void TEST_ASSERT_LITERAL_BOOL_EXPR(Expr* expr, int expected_value);
