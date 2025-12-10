#include "str_val.h"

#include "unity.h"

#include "value_t.h"

void test_str_val_true(void)
{
	Value bool_val = {.type = VAL_BOOL, .boolean = 1};
	const char* result = str_val(bool_val);
	TEST_ASSERT_EQUAL_STRING("true", result);
}

void test_str_val_bool_false(void)
{
	Value bool_val = {.type = VAL_BOOL, .boolean = 0};
	const char* result = str_val(bool_val);
	TEST_ASSERT_EQUAL_STRING("false", result);
}

void test_str_val_number(void)
{
	Value number_val = {.type = VAL_NUMBER, .number = 42.0};
	const char* result = str_val(number_val);
	TEST_ASSERT_EQUAL_STRING("42", result);
}

void test_str_val_number_decimal(void)
{
	Value number_val = {.type = VAL_NUMBER, .number = 3.14};
	const char* result = str_val(number_val);
	TEST_ASSERT_EQUAL_STRING("3.14", result);
}

void test_str_val_string(void)
{
	Value string_val = {.type = VAL_STRING, .string = "Hello, World!"};
	const char* result = str_val(string_val);
	TEST_ASSERT_EQUAL_STRING("Hello, World!", result);
}
