#include "unity.h"
#include "lexer.h"

TokenBuffer buffer;

void setUp(void) {
    init_token_buffer(&buffer);
}

void tearDown(void) {
    free_token_buffer(&buffer);
}

void test_lex_single_token_finds_token(void) {
    char line[] = "()";
    size_t current_pos = 0;

    lex_single_token(line, 34, sizeof(line) - 1, &buffer, &current_pos); // pass its address
    lex_single_token(line, 34, sizeof(line) - 1, &buffer, &current_pos);

    TEST_ASSERT_EQUAL(2, buffer.count);
    TEST_ASSERT_EQUAL_STRING("(", buffer.tokens[0].lexeme);
    TEST_ASSERT_EQUAL_STRING(")", buffer.tokens[1].lexeme);
}

