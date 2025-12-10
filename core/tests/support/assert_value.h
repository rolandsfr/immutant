
#pragma once

#include "unity.h"

// Assert that a Value is a number and has expected value
#define TEST_ASSERT_NUMBER_VALUE(val, expected)                                \
	do {                                                                       \
		TEST_ASSERT_EQUAL_INT(VAL_NUMBER, (val).type);                         \
		TEST_ASSERT_DOUBLE_WITHIN(1e-9, (expected), (val).number);             \
	} while (0)

// Assert that a Value is a boolean and has expected value
#define TEST_ASSERT_BOOL_VALUE(val, expected)                                  \
	do {                                                                       \
		TEST_ASSERT_EQUAL_INT(VAL_BOOL, (val).type);                           \
		TEST_ASSERT_EQUAL_INT((expected), (val).boolean);                      \
	} while (0)

// Assert that a Value is a string and equals expected C string
#define TEST_ASSERT_STRING_VALUE(val, expected)                                \
	do {                                                                       \
		TEST_ASSERT_EQUAL_INT(VAL_STRING, (val).type);                         \
		TEST_ASSERT_NOT_NULL((val).string);                                    \
		TEST_ASSERT_EQUAL_STRING((expected), (val).string);                    \
	} while (0)

// Assert that evaluation produced a runtime error with a specific code
#define TEST_ASSERT_RUNTIME_ERROR(err_ptr, expected_type)                      \
	do {                                                                       \
		TEST_ASSERT_NOT_NULL(err_ptr);                                         \
		TEST_ASSERT_EQUAL_INT((expected_type), (err_ptr)->type);               \
	} while (0)

// Assert that evaluation succeeded (no runtime error)
#define TEST_ASSERT_NO_RUNTIME_ERROR(err_ptr)                                  \
	do {                                                                       \
		TEST_ASSERT_NOT_NULL(err_ptr);                                         \
		TEST_ASSERT_EQUAL_INT(NO_ERROR, (err_ptr)->code);                      \
	} while (0)
