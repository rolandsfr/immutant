#pragma once

#include <stdio.h>

#include "array.h"
#include "ast_stmt.h"

typedef enum PurityType PurityType;
typedef struct ValueBuffer ValueBuffer;

typedef enum ValueType {
	VAL_NUMBER,
	VAL_BOOL,
	VAL_STRING,
	VAL_NULL,
	VAL_FN,
	VAL_NATIVE
} ValueType;

typedef struct Value {
	ValueType type;
	union {
		long double number;
		int boolean;
		char* string;
		struct Callable* callable;
	};

	enum MutabilityType mutability;
	enum PurityType purity;
} Value;

typedef struct Callable {
	Value (*call)(ValueBuffer* arguments);
	size_t arity;
} Callable;

ARR_DEFINE(Value, ValueBuffer)
