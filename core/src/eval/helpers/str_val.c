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
			snprintf(buffer, sizeof(buffer), "%Lf", value.number);

			// trim trailing zeros via ptr arithmetic
			char* p = buffer + strlen(buffer) - 1;
			while (p > buffer && *p == '0')
				p--;
			if (*p == '.')
				p--; // remove decimal if integer
			*(p + 1) = '\0';

			return buffer;
		}
		case VAL_BOOL:
			return value.boolean ? "true" : "false";
		case VAL_NATIVE:
			return "<native fn>";
		case VAL_FN:
			return "<fn>";
		default:
			return NULL;
	}
}
