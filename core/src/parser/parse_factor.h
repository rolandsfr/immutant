
#include "error_codes.h"
#include "lexer.h"

#include "ast/ast_cnstrct.h"

Expr* parse_factor(TokenBuffer* tokens, size_t* pos, ErrorCode* out_error);
