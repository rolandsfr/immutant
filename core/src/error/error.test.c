#include <stdlib.h>  

#include "unity.h"
#include "error.h" 

extern int hadError;

void setUp(void) {
    hadError = 0;
}

void test_format_error_creates_correct_message(void) {
    const char* received_message = format_error(98, "Some error has occured");
    const char* expected = "[line 98] Error: Some error has occured\n";
    TEST_ASSERT_EQUAL_STRING(expected, received_message);
    free((void*)received_message);
}

void test_error_sets_hadError_flag(void) {
    TEST_ASSERT_EQUAL_INT(0, hadError);
    error(42, "Test error");
    TEST_ASSERT_EQUAL_INT(1, hadError);
}