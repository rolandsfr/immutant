#pragma once

// Macro for defining evaluation function signatures for any Expr type
#define DEF_EVAL_EXPR(fn_name, ExprType)                                       \
	Value fn_name(struct ExprType* expr, RuntimeError* err)
