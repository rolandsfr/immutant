#pragma once

typedef enum {
    STMT_EXPR,
    STMT_VAR_DECL,
    STMT_FUN_DECL,
    STMT_RETURN,
    STMT_IF,
    STMT_WHILE,
    STMT_BLOCK
} StatementType;

typedef enum {
    EXPR_LITERAL_NUMBER,
    EXPR_LITERAL_STRING,
    EXPR_LITERAL_BOOL,
    EXPR_VARIABLE,
    EXPR_ASSIGN,
    EXPR_UNARY,
    EXPR_BINARY,
    EXPR_CALL,
} ExpressionType;

typedef struct Expr {
    ExpressionType type;
    int line;
} Expr;

typedef struct Stmt {
    StatementType type;
    int line;
} Stmt;
