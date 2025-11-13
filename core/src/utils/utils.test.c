#include "unity.h"
#include "utils.h"

void test_copy_char_should_copy_single_char_without_ref(void)
{
	const char* line = "hello world!";
	const char* first_letter = copy_char(&line[0]);

	TEST_ASSERT_EQUAL_CHAR(first_letter[0], 'h');
	TEST_ASSERT_EQUAL_CHAR(first_letter[1], '\0');
}
