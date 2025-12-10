#include <string.h>

#include "value_t.h"

int is_equal(Value left, Value right)
{
	if (left.type != right.type) {
		return 0;
	}

	switch (left.type) {
		case VAL_NUMBER:
			return left.number == right.number;
		case VAL_STRING:
			return (strcmp(left.string, right.string) == 0);
		case VAL_BOOL:
			return left.boolean == right.boolean;
		default:
			return 0;
	}
}
