#include "parse_expr.h"

#include <stdio.h>

#include "ast_cnstrct.h"
#include "ast_make_expr.h"
#include "lexer.h"
#include "parse_eq.h"
#include "parser_singnature.h"

DEF_PARSE_FN(parse_expr) { return parse_equality(tokens, pos, out_error); }
