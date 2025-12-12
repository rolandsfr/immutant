#pragma once

#include <stdio.h>

void error(int line, char* message);
void report_error(size_t line, const char* message, int* hadError);
const char* format_error(int line, const char* message);

typedef enum ErrorType {
	ERROR_NONE = -1,
	// Syntax errors
	SYNTAX_ERROR_UNTERMINATED_STRING,
	SYNTAX_ERROR_INVALID_NUMBER,
	SYNTAX_ERROR_INVALID_TOKEN,
	SYNTAX_ERROR_UNEXPECTED_TOKEN,
	SYNTAX_ERROR_MISSING_EXPRESSION,

	// Runtime errors
	RUNTIME_NO_ERROR,
	RUNTIME_DIVIDE_BY_ZERO,
	RUNTIME_UNDEFINED_VARIABLE,
	RUNTIME_UNEXPECTED_TYPE,

	// General errors
	ERROR_OUT_OF_MEMORY,
} ErrorType;

typedef struct Error {
	ErrorType type;
	size_t line;
	char message[128];
} Error;

typedef struct ErrorBuffer {
	size_t count;
	size_t capacity;
	Error* errors;
} ErrorBuffer;

void init_error_buffer(ErrorBuffer* buffer);

void add_error(ErrorBuffer* buffer, Error error);
