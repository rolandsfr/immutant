#pragma once

#include "unity.h"

#define TEST_ASSERT_ERROR(buffer, expectedType)                                \
	do {                                                                       \
		TEST_ASSERT_EQUAL_UINT_MESSAGE(1, (buffer).count,                      \
									   "Expected exactly one error");          \
		TEST_ASSERT_EQUAL_INT_MESSAGE((expectedType), (buffer).errors[0].type, \
									  "Error type mismatch");                  \
	} while (0)

#define TEST_ASSERT_ERRORS(buffer, expectedCount, ...)                         \
	do {                                                                       \
		ErrorType _expected[] = {__VA_ARGS__};                                 \
                                                                               \
		TEST_ASSERT_EQUAL_UINT_MESSAGE((expectedCount), (buffer).len,          \
									   "Error count mismatch");                \
                                                                               \
		for (size_t _i = 0; _i < (expectedCount); _i++) {                      \
			TEST_ASSERT_EQUAL_INT_MESSAGE(                                     \
				_expected[_i], (buffer).data[_i].type, "Error type mismatch"); \
		}                                                                      \
	} while (0)
