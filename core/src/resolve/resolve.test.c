#include <stdlib.h>

#include "unity.h"
#include "resolve.h"

TEST_SOURCE_FILE("lexer/lexer.c");

void test_resolve_string_should_resolve_empty_string(void)
{
    char* line  = "\"\";";
    size_t pos = 1;
    char* string = resolve_string(line, &pos);

    TEST_ASSERT_EQUAL_STRING("", string);
    free(string);

}

void test_resolve_string_should_resolve_and_return_full_string_until_closed_symbol(void)
{
    char* line = "\"once upon a time...\";";
    size_t pos = 1;
    char* string = resolve_string(line, &pos);

    TEST_ASSERT_EQUAL_STRING("once upon a time...", string);
}

void test_resolve_number_should_resolve_integer(void)
{
    char* line = "2204";
    size_t pos = 0;
    char* num = NULL;

    int is_resolved = resolve_number(line, &pos, &num);

    TEST_ASSERT_EQUAL_INT(1, is_resolved);

    if(is_resolved) {
        TEST_ASSERT_EQUAL_STRING("2204", num);
        TEST_ASSERT_EQUAL_INT(4, pos);
        free(num);
    }

}

void test_resolve_number_should_resolve_decimal(void)
{
    char* line = "0.043--";
    size_t pos = 0;
    char* num = NULL;

    int is_resolved = resolve_number(line, &pos, &num);

    TEST_ASSERT_EQUAL_INT(1, is_resolved);

    if(is_resolved) {
        TEST_ASSERT_EQUAL_STRING("0.043", num);
        TEST_ASSERT_EQUAL_INT(5, pos);
        free(num);
    }

}

void test_resolves_number_should_resolve_decimal_with_leading_dot(void)
{
    char* line = ".2323";
    size_t pos = 0;
    char* num = NULL;

    int is_resolved = resolve_number(line, &pos,&num);

    TEST_ASSERT_EQUAL_INT(1, is_resolved);

    if(is_resolved) {
        TEST_ASSERT_EQUAL_STRING(".2323", num);
        TEST_ASSERT_EQUAL_INT(5, pos);
    }

    free(num);
}

void test_resolves_number_should_not_resolve_single_dot(void)
{
    char* line = ".a";
    size_t pos = 0;
    char* num = NULL;

    int is_resolved = resolve_number(line, &pos, &num);

    TEST_ASSERT_EQUAL_INT(0, is_resolved);
    TEST_ASSERT_EQUAL_INT(0, pos);
}

void test_is_number_candidate_should_not_allow_chars_starting_whith_alpha(void)
{
    TEST_ASSERT_EQUAL_INT(is_number_candidate('y'), 0);
}

void test_is_number_candidate_should_not_allow_chars_starting_with_non_dot_non_number_chars(void)
{
    TEST_ASSERT_EQUAL_INT(is_number_candidate('+'), 0);
}

void test_is_number_candidate_should_not_allow_chars_starting_whith_leading_dot(void)
{
    TEST_ASSERT_EQUAL_INT(is_number_candidate('.'), 1); // is a good candidate but string itself not parsable
}
