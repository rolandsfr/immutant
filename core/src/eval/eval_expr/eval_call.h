#pragma once

#include "eval_singature.h"

typedef struct Env Env;
typedef struct Value Value;
typedef struct Error Error;
typedef struct CallExpr CallExpr;

DEF_EVAL_EXPR(eval_call, CallExpr);
