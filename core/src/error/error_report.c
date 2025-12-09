#include "error_report.h"

ErrorReport make_error_report(ErrorCode code, const char* message, size_t line)
{
	ErrorReport report;
	report.code = code;
	report.message = message;
	report.line = line;
	return report;
}

ErrorReport init_no_error_report()
{
	ErrorReport report;
	report.code = NO_ERROR;
	report.message = NULL;
	report.line = 0;
	return report;
}
