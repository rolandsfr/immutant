#pragma once

#include <stddef.h>

typedef struct TokenBuffer TokenBuffer;
typedef struct Error Error;

void sync_parse(TokenBuffer* tokens, size_t* pos, Error* out_error);
