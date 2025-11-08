#include "unity.h"
#include "lexer.h"

/** Util for copying char by value */
const char *copy_char(const char *c)
{
    if (c == NULL)
        return NULL;

    static char copy[2];
    copy[0] = *c;
    copy[1] = '\0';
    return copy;
}

TokenBuffer buffer;

void setUp(void)
{
    init_token_buffer(&buffer);
}

void tearDown(void)
{
    free_token_buffer(&buffer);
}


/** fn advance moves position forward by one */
void test_advance_moves_position_forward(void)
{
    char line[] = "abc";
    size_t current_pos = 0;

    const char* advanced = copy_char(advance(line, &current_pos));
    TEST_ASSERT_EQUAL_STRING("a", advanced);
    TEST_ASSERT_EQUAL_INT(current_pos, 1);
}

/** fn line_is_at_end tests if current position has reached end of line (string) */
void test_line_is_at_end_indicates_end_of_line(void)
{
    char *line = "abc";
    TEST_ASSERT_EQUAL_INT(0, line_is_at_end(line, 1));
    TEST_ASSERT_EQUAL_INT(1, line_is_at_end(line, 20));
}


/** fn peek tests if char is looked up without advancing position in source */
void test_peek_looks_up_char_without_advancing(void)
{
	char *line = "mutant a;";
	size_t current_pos = 6;

	const char* peeked = copy_char(peek(line, &current_pos));

	TEST_ASSERT_EQUAL_STRING(" ", peeked);
	TEST_ASSERT_EQUAL_INT(6, current_pos);
}

/** fn match_next returns true only if character at advanced position matches the expected one*/
void test_match_next_returns_bool_on_advanced_matched_char(void)
{
	char *line = "if()";
	size_t current_pos = 2;

	int has_advanced = match_next(line, &current_pos, "(");

	TEST_ASSERT_EQUAL_INT(1, has_advanced);
	TEST_ASSERT_EQUAL_INT(3, current_pos);
}
