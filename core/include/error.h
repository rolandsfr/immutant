#include <stdio.h>

void error(int line, char* message);
void report_error(size_t line, const char* message, int* hadError);
const char* format_error(int line, const char* message);
