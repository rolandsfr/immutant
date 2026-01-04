
#include "t_converters.h"

#include "ast_stmt.h"
#include "env.h"
#include "error.h"
#include "make_values.h"
#include "parser_singnature.h"
#include "str_val.h"
#include "value_t.h"

static Value native_toString(ValueBuffer* arguments, Context* context)
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

static Value native_typeOf(ValueBuffer* arguments, Context* context)
{
	Value arg = arguments->data[0];
	const char* type_str;

	switch (arg.type) {
		case VAL_NUMBER:
			type_str = "number";
			break;
		case VAL_BOOL:
			type_str = "boolean";
			break;
		case VAL_STRING:
			type_str = "string";
			break;
		case VAL_NULL:
			type_str = "null";
			break;
		case VAL_FN:
		case VAL_NATIVE:
			type_str = "function";
			break;
		default:
			type_str = "unknown";
			break;
	}

	return make_string((char*)type_str);
}

static void define_native_toString(Env* env)
{
	Callable* time_callable = malloc(sizeof(Callable));
	time_callable->arity = 1;
	time_callable->call = native_toString;

	Value time_value = (Value){
		.type = VAL_NATIVE,
		.callable = time_callable,
	};

	env_define_fn(env, "toString", time_value, PURE);
}

static void define_native_typeOf(Env* env)
{
	Callable* type_callable = malloc(sizeof(Callable));
	type_callable->arity = 1;
	type_callable->call = native_typeOf;

	Value type_value = (Value){
		.type = VAL_NATIVE,
		.callable = type_callable,
	};

	env_define_fn(env, "typeOf", type_value, PURE);
}

void define_native_converters(Env* env)
{
	define_native_toString(env);
	define_native_typeOf(env);
}
