#include <sys/time.h>

#include "ast_stmt.h"
#include "env.h"
#include "make_values.h"
#include "parser_singnature.h"
#include "value_t.h"

double time_in_ms()
{
	struct timeval tv;
	gettimeofday(&tv, NULL);

	return (double)tv.tv_sec * 1000.0 + (double)tv.tv_usec / 1000.0;
}

Value native_time(ValueBuffer* arguments)
{
	double current_time = time_in_ms();
	return make_number(current_time);
}

void define_native_time(Env* env)
{
	Callable* time_callable = malloc(sizeof(Callable));
	time_callable->arity = 0;
	time_callable->call = native_time;

	Value time_value;
	time_value.type = VAL_NATIVE;
	time_value.callable = time_callable;

	env_define_fn(env, "time", time_value, PURE);
}
