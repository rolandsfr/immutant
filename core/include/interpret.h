#pragma once

#include <stddef.h>

typedef struct Value Value;

typedef struct Values {
	Value* items;
	size_t count;
	size_t capacity;
} Values;

Values interpret_source(char* source, size_t length, size_t* line_nr);
