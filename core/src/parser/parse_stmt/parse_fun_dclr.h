#pragma once

#include <stddef.h>

typedef struct FunDeclStmt FunDeclStmt;
typedef struct TokenBuffer TokenBuffer;
typedef struct Error Error;

FunDeclStmt* parse_fun_decl(TokenBuffer* tokens, size_t* pos, Error* out_error);
