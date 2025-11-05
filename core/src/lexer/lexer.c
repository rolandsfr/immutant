#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "lexer.h"

Token create_token(enum TokenType type, const char* lexeme, size_t length, int line) {
    Token token;
    token.type = type;

    // copy char by value to avoid tests reading full string from pointer
    char* copy = malloc(length + 1);
    strncpy(copy, lexeme, length);
    copy[length] = '\0';

    token.lexeme = copy;
    token.length = length;
    token.line = line;
    return token;
}

void init_token_buffer(TokenBuffer* buffer) {
    buffer->count = 0;
    buffer->capacity = 64; 
    buffer->tokens = malloc(buffer->capacity * sizeof(Token));
}

void add_token(TokenBuffer* buffer, Token token) {
    // reallocate and increase total capacity if running out of buffer space
    if (buffer->count >= buffer->capacity) {
        buffer->capacity *= 2;
        buffer->tokens = realloc(buffer->tokens, buffer->capacity * sizeof(Token));

        if (buffer->tokens == NULL) {
            // handle memory allocation failure
            printf("Failed to allocate memory to save token\n");
            exit(1);
        }
    }

    buffer->tokens[buffer->count++] = token;
}

void free_token_buffer(TokenBuffer* buffer) {
    free(buffer->tokens);
    buffer->tokens = NULL;
    buffer->count = 0;
    buffer->capacity = 0;
}

void lex_single_token(char* line, int line_nr, size_t length, TokenBuffer* buffer, size_t* current_pos) {
    const char current_char = line[*current_pos];
    const char* lexeme = &line[*current_pos];
    switch (current_char) {
        case '(': 
            add_token(buffer, create_token(TOKEN_LEFT_PAREN, lexeme, 1, line_nr));
            (*current_pos)++;
            break;
        case ')':
            add_token(buffer, create_token(TOKEN_RIGHT_PAREN, lexeme, 1, line_nr));
            (*current_pos)++;
            break;
    }
}

void lex_line_tokens(char* line, size_t length, TokenBuffer* buffer, size_t start_pos, size_t current_pos) {

}

void scan_file_tokens(char* line, size_t length, TokenBuffer* buffer) {

}