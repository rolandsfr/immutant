#include "require_t.h"

#include "error.h"
#include "value_t.h"

DEF_REQUIRE_T_FN(require_number)
{
	if (value.type != VAL_NUMBER) {
		if (err) {
			*err = (Error){.type = RUNTIME_UNEXPECTED_TYPE,
						   .line = line,
						   .message = "Expected a number value"};
		}
		return 0;
	}

	return 1;
}

DEF_REQUIRE_T_FN(require_string)
{
	if (value.type != VAL_STRING) {
		if (err) {
			*err = (Error){.type = RUNTIME_UNEXPECTED_TYPE,
						   .line = line,
						   .message = "Expected a string value"};
		}
		return 0;
	}
	return 1;
}
DEF_REQUIRE_T_FN(require_bool)
{
	if (value.type != VAL_BOOL) {
		if (err) {
			*err = (Error){.type = RUNTIME_UNEXPECTED_TYPE,
						   .line = line,
						   .message = "Expected a boolean value"};
		}
		return 0;
	}
	return 1;
}

DEF_REQUIRE_T_SEVERAL_FN(require_numbers)
{
	if (value1.type != VAL_NUMBER || value2.type != VAL_NUMBER) {
		if (err) {
			*err = (Error){.type = RUNTIME_UNEXPECTED_TYPE,
						   .line = line,
						   .message = "Expected both values to be numbers"};
		}
		return 0;
	}

	return 1;
}

DEF_REQUIRE_T_SEVERAL_FN(check_strings)
{
	return (value1.type == VAL_STRING && value2.type == VAL_STRING);
}

DEF_REQUIRE_T_SEVERAL_FN(check_numbers)
{
	return (value1.type == VAL_NUMBER && value2.type == VAL_NUMBER);
}
