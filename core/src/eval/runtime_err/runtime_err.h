#pragma once

typedef enum RuntimeErrorType {
	RUNTIME_EXPECTED_DIFFERENT_TYPE,
	RUNTIME_INTERNAL_ERROR,
	RUNTIME_NO_ERROR
} RuntimeErrorType;

typedef struct RuntimeError {
	RuntimeErrorType type;
	const char* message;
} RuntimeError;
