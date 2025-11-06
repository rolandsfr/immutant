#include "unity.h"
#include "lexer.h"

TokenBuffer buffer;

void setUp(void)
{
    init_token_buffer(&buffer);
}

void tearDown(void)
{
    free_token_buffer(&buffer);
}

const char *copy_char(const char *c)
{
    if (c == NULL)
        return NULL;

    static char copy[2];
    copy[0] = *c;
    copy[1] = '\0';
    return copy;
}

void test_advance_moves_position_forward(void)
{
    char line[] = "abc";
    size_t current_pos = 0;

    const char* advanced = copy_char(advance(line, &current_pos));
    TEST_ASSERT_EQUAL_STRING("a", advanced);
}

void test_line_is_at_end_indicates_end_of_line(void)
{
    char *line = "abc";
    TEST_ASSERT_EQUAL_INT(0, line_is_at_end(line, 1));
    TEST_ASSERT_EQUAL_INT(1, line_is_at_end(line, 20));
}
