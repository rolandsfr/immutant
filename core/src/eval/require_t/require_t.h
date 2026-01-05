#pragma once

#include <stddef.h>

typedef struct Value Value;
typedef struct Error Error;

#define DEF_REQUIRE_T_FN(fn_name)                                              \
	int fn_name(Value value, Error* err, size_t line)

#define DEF_REQUIRE_T_SEVERAL_FN(fn_name)                                      \
	int fn_name(Value value1, Value value2, Error* err, size_t line)

DEF_REQUIRE_T_FN(require_number);
DEF_REQUIRE_T_FN(require_string);
DEF_REQUIRE_T_FN(require_bool);

DEF_REQUIRE_T_SEVERAL_FN(require_numbers);

DEF_REQUIRE_T_SEVERAL_FN(check_strings);
DEF_REQUIRE_T_SEVERAL_FN(check_numbers);
