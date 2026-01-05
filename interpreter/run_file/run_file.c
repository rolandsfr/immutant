
#include "run_file.h"

#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include "define_natives.h"
#include "error.h"
#include "eval.h"
#include "interpret.h"
#include "lexer.h"
#include "parser.h"
#include "str_val.h"
#include "types.h"

#include "env/env.h"

void run_file(const char* script_name)
{

	int fd = open(script_name, O_RDONLY);

	if (fd == -1) {
		printf("Error: Could not open file %s\n", script_name);
		return;
	}

	// Get file size
	off_t file_size = lseek(fd, 0, SEEK_END);
	lseek(fd, 0, SEEK_SET);

	if (file_size <= 0) {
		printf("Error: File is empty or cannot seek %s\n", script_name);
		close(fd);
		return;
	}

	// Read entire file into memory
	char* source = malloc(file_size + 1);
	if (!source) {
		printf("Error: Could not allocate memory for file\n");
		close(fd);
		return;
	}

	ssize_t bytes_read = read(fd, source, file_size);
	close(fd);

	if (bytes_read != file_size) {
		printf("Error: Could not read entire file\n");
		free(source);
		return;
	}

	source[file_size] = '\0';

	TokenBuffer token_buffer;
	init_token_buffer(&token_buffer);

	size_t lex_pos = 0;
	ErrorBuffer lex_errors;
	ErrorBuffer_init(&lex_errors);
	size_t line_nr = 1;

	scan_tokens(source, &line_nr, &token_buffer, &lex_pos, &lex_errors);
	free(source);

	if (had_errors(&lex_errors)) {
		report_errors(&lex_errors);
		free_token_buffer(&token_buffer);
		ErrorBuffer_free(&lex_errors);
		return;
	}

	ErrorBuffer errors;
	ErrorBuffer_init(&errors);
	Stmts stmts = parse(&token_buffer, &errors);

	if (had_errors(&errors)) {
		report_errors(&errors);
		ErrorBuffer_free(&errors);
		return;
	}

	Env* global_env = env_new(NULL, ENV_PURITY_UNSET);
	define_natives(global_env);
	Error runtime_error = {.type = ERROR_NONE, .line = 0};

	for (size_t i = 0; i < stmts.len; i++) {
		Stmt* stmt = stmts.data[i];
		Value value;

		eval(stmt, &runtime_error, &value, global_env);

		if (runtime_error.type != ERROR_NONE) {
			printf("Runtime error at line %zu: %s\n", runtime_error.line,
				   runtime_error.message);
			free_token_buffer(&token_buffer);
			int hadError = 1;
			report_error(runtime_error.line, runtime_error.message, &hadError);
			break;
		}
	}

	close(fd);
}
