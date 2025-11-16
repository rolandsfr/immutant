#include "unity.h"
#include "lexer.h"
#include "utils.h"
#include <assert.h>
#include <stdlib.h>


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

void test_scan_next_token_should_ignore_comments(void)
{
	char* line = "// some ordinary 123 comment";
	size_t pos = 0;
	size_t line_nr = 0;
	Token token;

	int token_emitted = scan_next_token(line, &pos, &line_nr, &token);
	TEST_ASSERT_EQUAL_INT(0, token_emitted);

}

void test_scan_next_token_should_detect_slash_not_comment(void)
{
	char* line = "/2";
	size_t pos = 0;
	size_t line_nr = 0;
	Token token;

	int token_produced = scan_next_token(line, &pos, &line_nr, &token);

	TEST_ASSERT_EQUAL_INT(1, token_produced);

	if(token_produced) {
		TEST_ASSERT_EQUAL_STRING("/", token.lexeme);
	}
}

void test_scan_next_token_should_increment_line_nr_on_newline(void)
{
	char* line = "\n";
	size_t pos = 0;
	size_t line_nr = 2;
	Token token;

	scan_next_token(line, &pos, &line_nr, &token);

	TEST_ASSERT_EQUAL_INT(3, line_nr);
}

void test_scan_next_token_should_not_increment_on_end_of_c_string(void)
{
	char* line = "";
	size_t pos = 0;
	size_t line_nr = 2;
	Token token;

	scan_next_token(line, &pos, &line_nr, &token);

	TEST_ASSERT_EQUAL_INT(2, line_nr);
}

void test_scan_next_token_should_detect_ordinary_single_char_lexeme(void)
{
	char* line = "()";
	size_t pos = 1;
	size_t line_nr = 0;
	Token token;

	int token_produced = scan_next_token(line, &pos, &line_nr, &token);

	TEST_ASSERT_EQUAL_INT(1, token_produced);

	if(token_produced) {
		TEST_ASSERT_EQUAL_STRING(")", token.lexeme);
	}
}

void test_scan_tokens_should_add_tokens_to_buffer_until_end_of_line(void)
{
	char* line = "{-() // some comment ++";
	size_t pos = 0;
	size_t line_nr = 0;

	TokenBuffer token_buffer;
	init_token_buffer(&token_buffer);

	scan_tokens(line, &line_nr, &token_buffer, &pos);

	TEST_ASSERT_EQUAL_INT(4, token_buffer.count);
	TEST_ASSERT_EQUAL_STRING(token_buffer.tokens[3].lexeme, ")");
	TEST_ASSERT_EQUAL_STRING(token_buffer.tokens[1].lexeme, "-");

	free_token_buffer(&token_buffer);
}
