#include <stdio.h>
#include <stdlib.h>

#include "ast_stmt.h"
#include "env.h"
#include "make_values.h"
#include "value_t.h"

static Value native_input(ValueBuffer* arguments, Context* context)
{
	char* line = NULL;
	size_t cap = 0;
	ssize_t read = getline(&line, &cap, stdin);

	if (read < 0) {
		free(line);
		return make_null();
	}

	if (read > 0 && line[read - 1] == '\n') {
		line[read - 1] = '\0';
	}

	return make_string(line);
}

void define_native_input(Env* env)
{
	Callable* input_callable = malloc(sizeof(Callable));
	input_callable->arity = 0;
	input_callable->call = native_input;

	Value input_value;
	input_value.type = VAL_NATIVE;
	input_value.callable = input_callable;

	env_define_fn(env, "input", input_value, IMPURE);
}
