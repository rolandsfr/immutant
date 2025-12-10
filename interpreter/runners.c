#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "interpret.h"

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

		// Value val = interpret_source(code_line, strlen(code_line));
	}
}
