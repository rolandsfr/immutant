#include "value_t.h"

Value make_number(double n) { return (Value){.type = VAL_NUMBER, .number = n}; }
Value make_bool(int b) { return (Value){.type = VAL_BOOL, .boolean = b}; }
Value make_string(char* s) { return (Value){.type = VAL_STRING, .string = s}; }

Value make_null() { return (Value){.type = VAL_NULL}; }
