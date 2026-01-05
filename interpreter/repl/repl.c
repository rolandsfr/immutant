#include "repl.h"

#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "buffer_block.h"
#include "interpret.h"
#include "str_val.h"
#include "types.h"

#include "env/env.h"
#include "native/define_natives.h"

extern Env* global_env;

void repl()
{
	if (!global_env) {
		global_env = env_new(NULL, ENV_PURITY_UNSET);
	}
	define_natives(global_env);

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

		char buffer[4096];
		size_t len = buffer_block(code_line, buffer, sizeof(buffer));

		Values val = interpret_source(buffer, len, &line_nr);

		for (size_t i = 0; i < val.count; i++) {
			Value v = val.items[i];
			if (v.type != VAL_NULL) {
				printf("%s\n", str_val(v));
			}
		}
	}
}
