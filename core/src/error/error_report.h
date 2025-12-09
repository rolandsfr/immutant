
#pragma once

#include <stddef.h>

#include "error_codes.h"

typedef struct ErrorReport {
	ErrorCode code;
	const char* message;
	size_t line;
} ErrorReport;

ErrorReport make_error_report(ErrorCode code, const char* message, size_t line);

ErrorReport init_no_error_report();
