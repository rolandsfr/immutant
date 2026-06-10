#pragma once

#include "ast_expr.h"
#include "parser_singnature.h"
#include "token_types.h"

typedef struct Expr Expr;
typedef struct Stmt Stmt;
typedef struct BinaryExpr BinaryExpr;
typedef struct VariableExpr VariableExpr;
typedef struct ErrorReport ErrorReport;

void TEST_ASSERT_UNARY_NUMBER_XPR(Expr* expr, enum TokenType expected_operator,
								  char* value);
void TEST_ASSERT_VAR_EXPR(Expr* expr, const char* expected_name);

void TEST_ASSERT_BINARY_EXPR(BinaryExpr* expr,
							 enum TokenType expected_operator);

void TEST_ASSERT_LITERAL_NUMBER_EXPR(Expr* expr, const char* expected_value);

void TEST_ASSERT_LITERAL_STRING_EXPR(Expr* expr, const char* expected_value);

void TEST_ASSERT_LITERAL_BOOL_EXPR(Expr* expr, int expected_value);

typedef struct SampleToken {
	enum TokenType type;
	const char* lexeme;
	size_t length;
} SampleToken;

typedef Stmt* (*ParseStmtFn)(TokenBuffer* tokens, size_t* pos, Error* out_error);
