#include "ast_stmt.h"
#include "value_t.h"

Value make_number(double n)
{
	return (Value){
		.type = VAL_NUMBER,
		.number = n,
		.mutability = MUTABILITY_UNSET,
	};
}
Value make_bool(int b)
{
	return (Value){
		.type = VAL_BOOL, .boolean = b, .mutability = MUTABILITY_UNSET};
}
Value make_string(char* s)
{
	return (Value){
		.type = VAL_STRING, .string = s, .mutability = MUTABILITY_UNSET};
}

Value make_null()
{
	return (Value){.type = VAL_NULL, .mutability = MUTABILITY_UNSET};
}
