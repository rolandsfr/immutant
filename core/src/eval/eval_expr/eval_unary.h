#include "eval_singature.h"

typedef struct Value Value;
typedef struct Error Error;
typedef struct UnaryExpr UnaryExpr;
typedef struct Env Env;

DEF_EVAL_EXPR(eval_unary, UnaryExpr);
