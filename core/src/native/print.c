#include "ast_stmt.h"
#include "env.h"
#include "make_values.h"
#include "parser_singnature.h"
#include "str_val.h"
#include "value_t.h"

Value native_print(ValueBuffer* arguments)
{
	Value arg = arguments->data[0];
	printf("%s\n", str_val(arg));
	return make_null();
}

void define_native_print(Env* env)
{
	Callable* time_callable = malloc(sizeof(Callable));
	time_callable->arity = 1;
	time_callable->call = native_print;

	Value time_value;
	time_value.type = VAL_NATIVE;
	time_value.callable = time_callable;

	env_define_fn(env, "print", time_value, PURE);
}
