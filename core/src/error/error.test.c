#include <stdlib.h>  

#include "unity.h"
#include "error.h" 

void test_format_error_creates_correct_message(void) {
    const char* received_message = format_error(98, "Some error has occured");
    const char* expected = "[line 98] Error: Some error has occured\n";
    TEST_ASSERT_EQUAL_STRING(expected, received_message);
    free((void*)received_message);
}

void test_report_error_sets_hadError_flag(void) {
    int hadError = 0;

    TEST_ASSERT_EQUAL_INT(0, hadError);
    report_error(42, "Test error", &hadError);
    TEST_ASSERT_EQUAL_INT(1, hadError);
}