#pragma once

#include <stdio.h>

#include "array.h"
#include "ast_expr.h"
#include "parser_singnature.h"

DEF_PARSE_FN(parse_call);

ARR_DEFINE(Expr*, ArgumentsArray);
