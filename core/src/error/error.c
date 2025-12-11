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

void init_error_buffer(ErrorBuffer* buffer)
{
	buffer->count = 0;
	buffer->capacity = 8;
	buffer->errors = malloc(buffer->capacity * sizeof(Error));
}

void add_error(ErrorBuffer* buffer, Error error)
{
	// reallocate and increase total capacity if running out of buffer space
	if (buffer->count >= buffer->capacity) {
		buffer->capacity *= 2;
		buffer->errors =
			realloc(buffer->errors, buffer->capacity * sizeof(Error));

		if (buffer->errors == NULL) {
			// handle memory allocation failure
			printf("Failed to allocate memory to save error\n");
			exit(1);
		}
	}

	buffer->errors[buffer->count++] = error;
}
