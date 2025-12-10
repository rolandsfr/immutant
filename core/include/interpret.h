#pragma once

#include <stddef.h>

typedef struct Value Value;

Value interpret_source(char* source, size_t length, size_t* line_nr);
