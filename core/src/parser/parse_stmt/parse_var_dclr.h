#pragma once

#include <stddef.h>

typedef struct TokenBuffer TokenBuffer;
typedef struct Error Error;
typedef struct VarDeclStmt VarDeclStmt;
enum MutabilityType;

VarDeclStmt* parse_var_decl(TokenBuffer* tokens, size_t* pos,
							enum MutabilityType mutability, Error* out_error);
