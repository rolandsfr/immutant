#include "unity.h"
#include "lexer.h"
#include "utils.h"


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

void test_scan_tokens_should_ignore_comments(void)
{
	char* line = "// some ordinary 123 comment \n";
	TokenBuffer buffer;

	init_token_buffer(&buffer);
	size_t pos = 0;
	scan_tokens(line, 0, 20, &buffer, &pos);

	TEST_ASSERT_EQUAL_INT(0, buffer.count);

	free_token_buffer(&buffer);
}

void test_scan_tokens_should_detect_slash_not_comment(void)
{
	char* line = "/2\n";
	TokenBuffer buffer;

	init_token_buffer(&buffer);
	size_t pos = 0;
	scan_tokens(line, 0, 20, &buffer, &pos);

	
	Token* tokens = buffer.tokens;
	Token first_token = tokens[0];
	const char* lexeme = copy_char(first_token.lexeme);

	TEST_ASSERT_EQUAL_STRING("/", lexeme);
	TEST_ASSERT_EQUAL_INT(1, buffer.count);

	free_token_buffer(&buffer);
}
