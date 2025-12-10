#pragma once

#include "eval_singature.h"

typedef struct BinaryExpr BinaryExpr;
typedef struct Value Value;
typedef struct RuntimeError RuntimeError;

DEF_EVAL_EXPR(eval_binary, BinaryExpr);
