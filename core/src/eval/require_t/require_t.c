#include "require_t.h"

#include "make_runtime_err.h"
#include "runtime_err.h"
#include "value_t.h"

int require_number(Value v, RuntimeError* err)
{
	if (v.type != VAL_NUMBER) {
		*err = make_runtime_error(RUNTIME_EXPECTED_DIFFERENT_TYPE,
								  "Expected a number value");
		return 0;
	}
	return 1;
}

int require_string(Value v, RuntimeError* err)
{
	if (v.type != VAL_STRING) {
		*err = make_runtime_error(RUNTIME_EXPECTED_DIFFERENT_TYPE,
								  "Expected a string value");
		return 0;
	}
	return 1;
}

int require_bool(Value v, RuntimeError* err)
{
	if (v.type != VAL_BOOL) {
		*err = make_runtime_error(RUNTIME_EXPECTED_DIFFERENT_TYPE,
								  "Expected a boolean value");
		return 0;
	}
	return 1;
}
