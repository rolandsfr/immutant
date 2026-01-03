
#include "t_converters.h"

#include "ast_stmt.h"
#include "env.h"
#include "error.h"
#include "make_values.h"
#include "parser_singnature.h"
#include "str_val.h"
#include "value_t.h"

Value native_toString(ValueBuffer* arguments, Context* context)
{
	Value arg = arguments->data[0];
	char* parsed_str = (char*)str_val(arg);

	if (parsed_str == NULL) {
		Error error = {.type = RUNTIME_INVALID_TYPE_CONVERSION,
					   .message = "Invalid type conversion.",
					   .line = context->line};
		context->error_out_tunnel->type = RUNTIME_UNEXPECTED_TYPE;
		return make_null();
	}

	return make_string(parsed_str);
}

void define_native_toString(Env* env)
{
	Callable* time_callable = malloc(sizeof(Callable));
	time_callable->arity = 1;
	time_callable->call = native_toString;

	Value time_value;
	time_value.type = VAL_NATIVE;
	time_value.callable = time_callable;

	env_define_fn(env, "toString", time_value, PURE);
}
