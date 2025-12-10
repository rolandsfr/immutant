#include "error.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int hadError = 0;

void error(int line, char* message) { report_error(line, message, &hadError); }

void report_error(size_t line, const char* message, int* hadError)
{
	const char* formatted_msg = format_error(line, message);
	printf("%s", formatted_msg);
	*hadError = 1;
}

const char* format_error(int line, const char* message)
{
	const char* prefix = "[line ";
	const char* middle = "] Error: ";

	size_t line_digits = snprintf(NULL, 0, "%d", line);
	size_t new_line_char_count = 2;
	size_t total_msg_len = strlen(prefix) + line_digits + strlen(middle) +
						   strlen(message) + new_line_char_count;

	char* buffer = malloc(total_msg_len);
	if (buffer == NULL)
		return NULL;

	snprintf(buffer, total_msg_len, "%s%d%s%s\n", prefix, line, middle,
			 message);
	return buffer;
}
