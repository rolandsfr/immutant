#include "resolve.h"
#include "lexer/lexer.c"

#include "unity.h"

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
    char* line  = "\"once upon a time...\";";
    size_t pos = 1;
    char* string = resolve_string(line, &pos);

    TEST_ASSERT_EQUAL_STRING("once upon a time...", string);
}
