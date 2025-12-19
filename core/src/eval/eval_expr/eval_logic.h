#pragma once

#include "eval_singature.h"

typedef struct LogicalExpr LogicalExpr;
typedef struct Error Error;
typedef struct Env Env;
typedef struct Value Value;

DEF_EVAL_EXPR(eval_logic, LogicalExpr);
