#include "test_expr.h"

#include "unity.h"

#include "ast_expr.h"

void TEST_ASSERT_UNARY_NUMBER_XPR(Expr* expr, enum TokenType expected_operator,
								  char* value)
{
	TEST_ASSERT_NOT_NULL(expr);
	TEST_ASSERT_EQUAL_INT(EXPR_UNARY, expr->type);
	UnaryExpr* unary_expr = (UnaryExpr*)expr;
	TEST_ASSERT_EQUAL_INT(expected_operator, unary_expr->operator);
	TEST_ASSERT_EQUAL_STRING(value, ((NumberExpr*)unary_expr->operand)->value);
}

void TEST_ASSERT_VAR_EXPR(Expr* expr, const char* expected_name)
{
	TEST_ASSERT_NOT_NULL(expr);
	TEST_ASSERT_EQUAL_INT(EXPR_VARIABLE, expr->type);
	TEST_ASSERT_EQUAL_STRING(expected_name, ((VariableExpr*)expr)->name);
}

void TEST_ASSERT_BINARY_EXPR(BinaryExpr* expr, enum TokenType expected_operator)
{
	TEST_ASSERT_NOT_NULL(expr);
	TEST_ASSERT_EQUAL_INT(EXPR_BINARY, expr->base.type);
	TEST_ASSERT_EQUAL_INT(expected_operator, expr->operator);
}

void TEST_ASSERT_LITERAL_NUMBER_EXPR(Expr* expr, const char* expected_value)
{
	TEST_ASSERT_NOT_NULL(expr);
	TEST_ASSERT_EQUAL_INT(EXPR_LITERAL_NUMBER, expr->type);
	TEST_ASSERT_EQUAL_STRING(expected_value, ((NumberExpr*)expr)->value);
}

void TEST_ASSERT_LITERAL_STRING_EXPR(Expr* expr, const char* expected_value)
{
	TEST_ASSERT_NOT_NULL(expr);
	TEST_ASSERT_EQUAL_INT(EXPR_LITERAL_STRING, expr->type);
	TEST_ASSERT_EQUAL_STRING(expected_value, ((StringExpr*)expr)->value);
}

void TEST_ASSERT_LITERAL_BOOL_EXPR(Expr* expr, int expected_value)
{
	TEST_ASSERT_NOT_NULL(expr);
	TEST_ASSERT_EQUAL_INT(EXPR_LITERAL_BOOL, expr->type);
	TEST_ASSERT_EQUAL_INT(expected_value, ((BooleanExpr*)expr)->value);
}
