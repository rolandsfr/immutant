#include <stdio.h>
#include <string.h>

#include "value_t.h"

const char* str_val(Value value)
{
	switch (value.type) {
		case VAL_STRING:
			return value.string;
		case VAL_NUMBER: {
			static char buffer[32];
			snprintf(buffer, sizeof(buffer), "%g", value.number);
			return buffer;
		}
		case VAL_BOOL:
			return value.boolean ? "true" : "false";
		default:
			return NULL;
	}
}
