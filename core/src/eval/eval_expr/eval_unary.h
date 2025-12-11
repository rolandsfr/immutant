#include "eval_singature.h"

typedef struct Value Value;
typedef struct Error Error;
typedef struct UnaryExpr UnaryExpr;

DEF_EVAL_EXPR(eval_unary, UnaryExpr);
