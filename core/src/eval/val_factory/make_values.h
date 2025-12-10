#pragma once

typedef struct Value Value;

Value make_number(double n);
Value make_bool(int b);
Value make_string(char* s);
Value make_null();
