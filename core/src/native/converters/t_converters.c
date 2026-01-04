
#include "t_converters.h"

#include "ast_stmt.h"
#include "env.h"
#include "error.h"
#include "make_values.h"
#include "parser_singnature.h"
#include "str_val.h"
#include "value_t.h"

// Write an error into the caller-provided error slot, if available.
static void set_runtime_error(Context* context, ErrorType type,
							  const char* message)
{
	if (context && context->error_out_tunnel) {
		Error* out = context->error_out_tunnel;
		*out = (Error){.type = type, .line = context->line, .message = {0}};
		snprintf(out->message, sizeof(out->message), "%s", message);
	}
}

static Value native_toString(ValueBuffer* arguments, Context* context)
{
	Value arg = arguments->data[0];
	char* parsed_str = (char*)str_val(arg);

	if (parsed_str == NULL) {
		set_runtime_error(context, RUNTIME_INVALID_TYPE_CONVERSION,
						  "Invalid type conversion.");
		return make_null();
	}

	return make_string(parsed_str);
}

static Value native_toNumber(ValueBuffer* arguments, Context* context)
{
	Value arg = arguments->data[0];
	switch (arg.type) {
		case VAL_NUMBER:
			return arg;
		case VAL_BOOL:
			return make_number(arg.boolean ? 1.0 : 0.0);
		case VAL_STRING: {

			char* str = (char*)str_val(arg);
			char* endptr;
			double number = strtod(str, &endptr);

			if (endptr == str) {
				set_runtime_error(context, RUNTIME_INVALID_TYPE_CONVERSION,
								  "Invalid type conversion.");
				return make_null();
			}

			return make_number(number);
		}
		default:
			set_runtime_error(context, RUNTIME_INVALID_TYPE_CONVERSION,
							  "Invalid type conversion.");
			return make_null();
	}
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

void define_native_toNumber(Env* env)
{
	Callable* to_number_callable = malloc(sizeof(Callable));
	to_number_callable->arity = 1;
	to_number_callable->call = native_toNumber;

	Value to_number_value = (Value){
		.type = VAL_NATIVE,
		.callable = to_number_callable,
	};

	env_define_fn(env, "toNumber", to_number_value, PURE);
}

void define_native_converters(Env* env)
{
	define_native_toString(env);
	define_native_typeOf(env);
	define_native_toNumber(env);
}
