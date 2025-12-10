#pragma once

typedef enum RuntimeErrorType {
	RUNTIME_EXPECTED_DIFFERENT_TYPE,
	RUNTIME_DIVIDE_BY_ZERO,
	RUNTIME_INTERNAL_ERROR,
	RUNTIME_NO_ERROR
} RuntimeErrorType;

typedef struct RuntimeError {
	RuntimeErrorType type;
	const char* message;
} RuntimeError;
