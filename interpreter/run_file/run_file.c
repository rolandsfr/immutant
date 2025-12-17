
#include "run_file.h"

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
