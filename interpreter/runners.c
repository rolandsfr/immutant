#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "interpret.h"
#include "str_val.h"
#include "types.h"

void run_file(const char* script_name)
{
	int fd = open(script_name, O_RDONLY);

	if (fd == -1) {
		printf("Error: Could not open file %s\n", script_name);
	}

	while (1) {
		char buffer[1024];
		ssize_t bytes_read = read(fd, buffer, sizeof(buffer) - 1);
		if (bytes_read <= 0) {
			break; // EOF or error
		}
		buffer[bytes_read] = '\0'; // Null-terminate the string
	}

	close(fd);
}

void run_interactive()
{
	char code_line[256];
	size_t line_nr = 1;

	while (1) {
		printf(">>> ");
		if (fgets(code_line, sizeof(code_line), stdin) == NULL) {
			break; // exit on EOF
		}

		// remove trailing newline
		code_line[strcspn(code_line, "\n")] = '\0';

		// stop on empty line
		if (strlen(code_line) == 0) {
			break;
		}

		// stop on "exit" command
		if (strcmp(code_line, "exit") == 0) {
			break;
		}

		// REPL buffer for multi-line blocks
		char repl_buffer[4096] = {0};
		size_t repl_len = 0;
		int brace_count = 0;

		// Append first line
		strcpy(repl_buffer, code_line);
		repl_len = strlen(repl_buffer);

		// Count braces in first line
		for (char* p = code_line; *p; p++) {
			if (*p == '{')
				brace_count++;
			if (*p == '}')
				brace_count--;
		}

		// If braces are unbalanced, keep reading lines
		while (brace_count > 0) {
			printf("... "); // continuation prompt

			if (fgets(code_line, sizeof(code_line), stdin) == NULL) {
				break; // EOF
			}

			// Remove trailing newline
			code_line[strcspn(code_line, "\n")] = '\0';

			// Append to REPL buffer
			size_t line_len = strlen(code_line);
			if (repl_len + line_len + 2 < sizeof(repl_buffer)) {
				repl_buffer[repl_len++] = '\n';
				strcpy(repl_buffer + repl_len, code_line);
				repl_len += line_len;
			} else {
				printf("Error: REPL buffer overflow\n");
				break;
			}

			// Update brace count
			for (char* p = code_line; *p; p++) {
				if (*p == '{')
					brace_count++;
				if (*p == '}')
					brace_count--;
			}
		}

		Values val = interpret_source(repl_buffer, repl_len, &line_nr);
		for (size_t i = 0; i < val.count; i++) {
			Value v = val.items[i];
			if (v.type != VAL_NULL) {
				printf("%s\n", str_val(v));
			}
		}
	}
}
