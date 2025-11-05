#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "lexer.h"

Token create_token(enum TokenType type, const char *lexeme, size_t length, int line)
{
    Token token;
    token.type = type;

    // copy char by value to avoid tests reading full string from pointer
    char *copy = malloc(length + 1);
    strncpy(copy, lexeme, length);
    copy[length] = '\0';

    token.lexeme = copy;
    token.length = length;
    token.line = line;
    return token;
}

void init_token_buffer(TokenBuffer *buffer)
{
    buffer->count = 0;
    buffer->capacity = 64;
    buffer->tokens = malloc(buffer->capacity * sizeof(Token));
}

void add_token(TokenBuffer *buffer, Token token)
{
    // reallocate and increase total capacity if running out of buffer space
    if (buffer->count >= buffer->capacity)
    {
        buffer->capacity *= 2;
        buffer->tokens = realloc(buffer->tokens, buffer->capacity * sizeof(Token));

        if (buffer->tokens == NULL)
        {
            // handle memory allocation failure
            printf("Failed to allocate memory to save token\n");
            exit(1);
        }
    }

    buffer->tokens[buffer->count++] = token;
}

void free_token_buffer(TokenBuffer *buffer)
{
    free(buffer->tokens);
    buffer->tokens = NULL;
    buffer->count = 0;
    buffer->capacity = 0;
}

int str_len_with_newline(const char *str)
{
    size_t len = 0;
    while (str[len] != '\0' && str[len] != '\n')
    {
        len++;
    }
    return len;
}

/** looks up ahead 1 char from current position without consuming the character */
const char *peek(char *line, size_t *current_pos)
{
    if (line_is_at_end(line, *current_pos))
        return NULL;

    return &line[*current_pos];
}

/* returns character at the current position and advances the current position by one */
const char *advance(char *line, size_t *current_pos)
{
    if (line_is_at_end(line, *current_pos))
        return NULL;

    (*current_pos)++;
    return &line[*current_pos - 1];
}

/** advances position if character at current position matches expected one */
int match_next(char *line, size_t *current_pos, char expected)
{
    if (line[*current_pos] == expected)
    {
        (*current_pos)++;
        return 1;
    }

    return 0;
}

int line_is_at_end(char *line, size_t current_pos)
{
    return current_pos >= strlen(line);
}

short int scan_tokens(char *line, int line_nr, size_t length, Token *out_token, size_t *current_pos)
{
    const char current_char = line[*current_pos];
    const char *lexeme = &line[*current_pos];

    switch (current_char)
    {
    case '(':
        *out_token = create_token(TOKEN_LEFT_PAREN, lexeme, 1, line_nr);
        break;
    case ')':
        *out_token = create_token(TOKEN_RIGHT_PAREN, lexeme, 1, line_nr);
        break;
    case '{':
        *out_token = create_token(TOKEN_LEFT_BRACE, lexeme, 1, line_nr);
        break;
    case '}':
        *out_token = create_token(TOKEN_RIGHT_BRACE, lexeme, 1, line_nr);
        break;
    }

    return 0;
}
