
#include "lexer.h"

#include <assert.h>
#include <stdlib.h>

#include "unity.h"

#include "assert_error.h"
#include "error.h"
#include "utils.h"

#include "resolve.h"

/** fn advance moves position forward by one */
void test_advance_moves_position_forward(void)
{
	char line[] = "abc";
	size_t current_pos = 0;

	const char* advanced = copy_char(advance(line, &current_pos));
	TEST_ASSERT_EQUAL_STRING("a", advanced);
	TEST_ASSERT_EQUAL_INT(current_pos, 1);
}

/** fn line_is_at_end tests if current position has reached end of line
(string)
 */
void test_line_is_at_end_indicates_end_of_line(void)
{
	char* line = "abc";
	TEST_ASSERT_EQUAL_INT(0, line_is_at_end(line, 1));
	TEST_ASSERT_EQUAL_INT(1, line_is_at_end(line, 20));
}

/** fn peek tests if char is looked up without advancing position in source
 */
void test_peek_looks_up_char_without_advancing(void)
{
	char* line = "mutant a;";

	size_t current_pos = 6;

	const char* peeked = copy_char(peek(line, &current_pos));

	TEST_ASSERT_EQUAL_STRING(" ", peeked);
	TEST_ASSERT_EQUAL_INT(6, current_pos);
}

void test_peek_expect_matches_expected_without_advancing(void)
{
	char* line = "32=";

	size_t current_pos = 2;

	int matches_peeked = peek_expect(line, &current_pos, "=12", 0);

	TEST_ASSERT_EQUAL_INT(1, matches_peeked);
}

void test_peek_expect_does_not_match_at_the_end_of_line(void)
{
	char* line = "32=";

	size_t current_pos = 3;

	int matches_peeked = peek_expect(line, &current_pos, "=12", 0);

	TEST_ASSERT_EQUAL_INT(0, matches_peeked);
}

/** fn match_next returns true only if character at advanced position matches
 * the expected one*/
void test_match_next_returns_bool_on_advanced_matched_char(void)
{
	char* line = "if()";
	size_t current_pos = 2;

	int has_advanced = match_next(line, &current_pos, "(");

	TEST_ASSERT_EQUAL_INT(1, has_advanced);
	TEST_ASSERT_EQUAL_INT(3, current_pos);
}

void test_scan_next_token_should_ignore_comments(void)
{
	char* line = "//some ordinary 123 comment";
	size_t pos = 0;
	size_t line_nr = 0;
	Token token;

	int token_emitted;

	token_emitted = scan_next_token(line, &pos, &line_nr, &token, NULL);

	TEST_ASSERT_EQUAL_INT(0, token_emitted);
	TEST_ASSERT_EQUAL_INT(27, pos);
	TEST_ASSERT_EQUAL_INT(1, line_is_at_end(line, pos));
}

void test_scan_next_token_should_detect_slash_not_comment(void)
{
	char* line = "/2";
	size_t pos = 0;
	size_t line_nr = 0;
	Token token;

	int token_produced = scan_next_token(line, &pos, &line_nr, &token, NULL);

	TEST_ASSERT_EQUAL_INT(1, token_produced);
	TEST_ASSERT_EQUAL_INT(1, pos);

	if (token_produced) {
		TEST_ASSERT_EQUAL_STRING("/", token.lexeme);
	}
}

void test_scan_next_token_should_increment_line_nr_on_newline(void)
{
	char* line = "\n";
	size_t pos = 0;
	size_t line_nr = 2;
	Token token;

	scan_next_token(line, &pos, &line_nr, &token, NULL);

	TEST_ASSERT_EQUAL_INT(3, line_nr);
}

void test_scan_next_token_should_not_increment_on_end_of_c_string(void)
{
	char* line = "";
	size_t pos = 0;
	size_t line_nr = 2;
	Token token;

	scan_next_token(line, &pos, &line_nr, &token, NULL);

	TEST_ASSERT_EQUAL_INT(2, line_nr);
}

void test_scan_next_token_should_detect_ordinary_single_char_lexeme(void)
{
	char* line = "()";
	size_t pos = 1;
	size_t line_nr = 0;
	Token token;

	int token_produced = scan_next_token(line, &pos, &line_nr, &token, NULL);

	TEST_ASSERT_EQUAL_INT(1, token_produced);

	if (token_produced) {
		TEST_ASSERT_EQUAL_STRING(")", token.lexeme);
		TEST_ASSERT_EQUAL_INT(TOKEN_RIGHT_PAREN, token.type);
	}
}

void test_scan_next_token_should_detect_multichar_operators(void)
{
	char* line = "!=";
	size_t pos = 0;
	size_t line_nr = 0;
	Token token;

	int token_produced = scan_next_token(line, &pos, &line_nr, &token, NULL);

	TEST_ASSERT_EQUAL_INT(1, token_produced);

	if (token_produced) {
		TEST_ASSERT_EQUAL_STRING("!=", token.lexeme);
		TEST_ASSERT_EQUAL_INT(TOKEN_BANG_EQUAL, token.type);
	}
}

void test_scan_next_token_should_detect_multichar_operators_with_single_char(
	void)
{
	char* line = "1>";
	size_t pos = 1;
	size_t line_nr = 0;
	Token token;

	int token_produced = scan_next_token(line, &pos, &line_nr, &token, NULL);

	TEST_ASSERT_EQUAL_INT(1, token_produced);

	if (token_produced) {
		TEST_ASSERT_EQUAL_STRING(">", token.lexeme);
		TEST_ASSERT_EQUAL_INT(TOKEN_GREATER, token.type);
	}
}

void test_scan_next_token_should_resolve_strings(void)
{
	char* line = "\"sunny day\"";
	size_t pos = 0;
	size_t line_nr = 0;
	Token token;

	int token_produced = scan_next_token(line, &pos, &line_nr, &token, NULL);

	TEST_ASSERT_EQUAL_INT(1, token_produced);

	if (token_produced) {
		TEST_ASSERT_EQUAL_STRING("sunny day", token.lexeme);
		TEST_ASSERT_EQUAL_INT(TOKEN_STRING, token.type);
	}
}

void test_scan_next_token_should_not_produce_string_if_line_ends_with_double_quote(
	void)
{
	char* line = "\"";
	size_t pos = 0;
	size_t line_nr = 0;
	Token token;

	Error out_error;
	int token_produced =
		scan_next_token(line, &pos, &line_nr, &token, &out_error);

	TEST_ASSERT_EQUAL_INT(0, token_produced);
	TEST_ASSERT_EQUAL_INT(SYNTAX_ERROR_UNTERMINATED_STRING, out_error.type);
}

void test_scan_next_token_should_return_syntax_error_if_invalid_token_found(
	void)
{
	char* line = "@";
	size_t pos = 0;
	size_t line_nr = 0;
	Token token;

	Error out_error;
	int token_produced =
		scan_next_token(line, &pos, &line_nr, &token, &out_error);

	TEST_ASSERT_EQUAL_INT(0, token_produced);
	TEST_ASSERT_EQUAL_INT(SYNTAX_ERROR_INVALID_TOKEN, out_error.type);
}

void test_scan_next_token_should_resolve_full_double_precision_number(void)
{
	char* line = "39.456723asd=as";
	size_t pos = 0;
	size_t line_nr = 0;
	Token token;

	int token_produced = scan_next_token(line, &pos, &line_nr, &token, NULL);
	TEST_ASSERT_EQUAL_INT(1, token_produced);

	if (token_produced) {
		TEST_ASSERT_EQUAL_STRING("39.456723", token.lexeme);
		TEST_ASSERT_EQUAL_INT(TOKEN_NUMBER, token.type);
	}
}

void test_scan_next_token_should_resolve_whole_number(void)
{
	char* line = "159";
	size_t pos = 0;
	size_t line_nr = 0;
	Token token;

	int token_produced = scan_next_token(line, &pos, &line_nr, &token, NULL);
	TEST_ASSERT_EQUAL_INT(1, token_produced);

	if (token_produced) {
		TEST_ASSERT_EQUAL_STRING("159", token.lexeme);
		TEST_ASSERT_EQUAL_INT(TOKEN_NUMBER, token.type);
	}
}

void test_scan_next_token_should_resolve_zero(void)
{
	char* line = "0";
	size_t pos = 0;
	size_t line_nr = 0;
	Token token;

	int token_produced = scan_next_token(line, &pos, &line_nr, &token, NULL);
	TEST_ASSERT_EQUAL_INT(1, token_produced);

	if (token_produced) {
		TEST_ASSERT_EQUAL_STRING("0", token.lexeme);
		TEST_ASSERT_EQUAL_INT(TOKEN_NUMBER, token.type);
	}
}

void test_scan_tokens_should_add_tokens_to_buffer_until_end_of_line(void)
{
	char* line = "{-() // some comment (";
	size_t pos = 0;
	size_t line_nr = 0;

	TokenBuffer token_buffer;
	init_token_buffer(&token_buffer);

	scan_tokens(line, &line_nr, &token_buffer, &pos, NULL);

	TEST_ASSERT_EQUAL_INT(4, token_buffer.count);
	TEST_ASSERT_EQUAL_STRING(token_buffer.tokens[3].lexeme, ")");
	TEST_ASSERT_EQUAL_STRING(token_buffer.tokens[1].lexeme, "-");

	free_token_buffer(&token_buffer);
}

void test_scan_tokens_should_scan_all_but_return_errors(void)
{
	char* line = "3 + @ - $()";
	size_t pos = 0;
	size_t line_nr = 0;

	TokenBuffer token_buffer;
	init_token_buffer(&token_buffer);

	ErrorBuffer error_buffer;
	init_error_buffer(&error_buffer);
	scan_tokens(line, &line_nr, &token_buffer, &pos, &error_buffer);

	TEST_ASSERT_EQUAL_INT(5, token_buffer.count);

	TEST_ASSERT_EQUAL_STRING(token_buffer.tokens[0].lexeme, "3");
	TEST_ASSERT_EQUAL_STRING(token_buffer.tokens[1].lexeme, "+");
	TEST_ASSERT_EQUAL_STRING(token_buffer.tokens[2].lexeme, "-");
	TEST_ASSERT_EQUAL_STRING(token_buffer.tokens[3].lexeme, "(");
	TEST_ASSERT_EQUAL_STRING(token_buffer.tokens[4].lexeme, ")");

	TEST_ASSERT_ERRORS(error_buffer, 2, SYNTAX_ERROR_INVALID_TOKEN,
					   SYNTAX_ERROR_INVALID_TOKEN);

	free_token_buffer(&token_buffer);
}

void test_scan_next_token_should_resolve_identifier(void)
{

	char* line = "_immutant";
	size_t pos = 0;
	size_t line_nr = 0;
	Token token;

	int token_produced = scan_next_token(line, &pos, &line_nr, &token, NULL);

	TEST_ASSERT_EQUAL_INT(1, token_produced);

	if (token_produced) {
		TEST_ASSERT_EQUAL_STRING("_immutant", token.lexeme);
		TEST_ASSERT_EQUAL_INT(TOKEN_IDENTIFIER, token.type);
	}
}

void test_scan_next_token_should_resolve_keyword(void)
{
	char* line = "else";
	size_t pos = 0;
	size_t line_nr = 0;
	Token token;

	int token_produced = scan_next_token(line, &pos, &line_nr, &token, NULL);

	TEST_ASSERT_EQUAL_INT(1, token_produced);

	if (token_produced) {
		TEST_ASSERT_EQUAL_STRING("else", token.lexeme);
		TEST_ASSERT_EQUAL_INT(TOKEN_ELSE, token.type);
	}
}
