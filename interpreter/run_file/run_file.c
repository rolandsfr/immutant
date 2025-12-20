
#include "run_file.h"

#include <fcntl.h>
#include <stdio.h>
#include <string.h>
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
	}

	TokenBuffer token_buffer;
	init_token_buffer(&token_buffer);

	size_t lex_pos = 0;
	ErrorBuffer lex_errors;
	ErrorBuffer_init(&lex_errors);
	size_t line_nr = 1;

	while (1) {
		char buffer[1024];
		ssize_t bytes_read = read(fd, buffer, sizeof(buffer) - 1);
		if (bytes_read <= 0) {
			break; // EOF or error
		}
		buffer[bytes_read] = '\0'; // Null-terminate the string
		scan_tokens(buffer, &line_nr, &token_buffer, &lex_pos, &lex_errors);
	}

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

	Env* global_env = env_new(NULL);
	define_natives(global_env);

	for (size_t i = 0; i < stmts.len; i++) {
		Error runtime_error = {-1};
		Stmt* stmt = stmts.data[i];
		Value value;

		eval(stmt, &runtime_error, &value, global_env);

		if (runtime_error.type != ERROR_NONE) {
			free_token_buffer(&token_buffer);
			int hadError = 1;
			report_error(runtime_error.line, runtime_error.message, &hadError);
			continue;
		}
	}

	close(fd);
}
