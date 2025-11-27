#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "lexer.h"
#include "error.h"

#include "resolve.h"

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

Token create_multi_char_token(char *line, size_t *pos, int line_nr, enum TokenType single, enum TokenType double_type, char expected_next) {
    if (match_next(line, pos, &expected_next)) {
        return create_token(double_type, line + *pos - 2, 2, line_nr);
    }

    return create_token(single, line + *pos - 1, 1, line_nr);
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

/** looks up ahead 1 char from current position without consuming the character */
const char *peek(char *line, size_t *current_pos)
{
    if (line_is_at_end(line, *current_pos))
        return "\0";

    return &line[*current_pos];
}

/** looks up ahead 2 chars from current position without consuming the characters */
const char *peek_next(char *line, size_t current_pos)
{
    if(line_is_at_end(line, current_pos + 1)) return "\0";

    return &line[current_pos + 1];
}

int peek_expect(char* line, size_t* current_pos, const char* expect_charset, int invert_expect) {
    const char* peeked = peek(line, current_pos);

    if (*peeked == '\0')
        return 0;

    for(int i = 0; expect_charset[i] != '\0'; i++)  {
        if(expect_charset[i] == *peeked) return invert_expect ? 0 : 1;
    }

    return invert_expect ? 1 : 0;
}

/* returns character at the current position and advances the current position by one */
const char *advance(char *line, size_t *current_pos)
{
    if (line_is_at_end(line, *current_pos))
        return NULL;

    (*current_pos)++;
    return &line[*current_pos - 1];
}

const char* advance_until(char* line, size_t* pos, const char* charset, int consume_last)
{
    const char* ch;
    while(peek_expect(line, pos, charset, 1)) {
        ch = advance(line, pos);
    }

    if(consume_last) {
        ch = advance(line, pos);
    }

    return ch;
}

/** advances position if character at current position matches expected one and returns bool
 * if such advancement was made
*/
int match_next(char *line, size_t *current_pos, const char* expected)
{
    if (line[*current_pos] == *expected)
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


/** returns lexeme of the next closes token */
int scan_next_token(char *line, size_t *current_pos, size_t* line_nr, Token* token)
{
    const char* current_char = advance(line, current_pos);
    if(!current_char) return 0;

    switch (*current_char)
    {
        case '(':
            *token = create_token(TOKEN_LEFT_PAREN, current_char, 1, *line_nr);
            break;
        case ')':
            *token = create_token(TOKEN_RIGHT_PAREN, current_char, 1, *line_nr);
            break;
        case '{':
            *token = create_token(TOKEN_LEFT_BRACE, current_char, 1, *line_nr);
            break;
        case '}':
            *token = create_token(TOKEN_RIGHT_BRACE, current_char, 1, *line_nr);
            break;
        case '.':
            *token = create_token(TOKEN_DOT, current_char, 1, *line_nr);
            break;
        case ',':
            *token = create_token(TOKEN_COMMA, current_char, 1, *line_nr);
            break;
        case '-':
            *token = create_token(TOKEN_MINUS, current_char, 1, *line_nr);
            break;
        case '+':
            *token = create_token(TOKEN_PLUS, current_char, 1, *line_nr);
            break;
        case ';':
            *token = create_token(TOKEN_SEMICOLON, current_char, 1, *line_nr);
            break;
        case '*':
            *token = create_token(TOKEN_STAR, current_char, 1, *line_nr);
            break;
        
        // multi-char operators
        case '!':
            *token = create_multi_char_token(line, current_pos, *line_nr, TOKEN_BANG, TOKEN_BANG_EQUAL, '=');
            break;
        case '=':
            *token = create_multi_char_token(line, current_pos, *line_nr, TOKEN_EQUAL, TOKEN_EQUAL_EQUAL, '=');
            break;
        case '>':
            *token = create_multi_char_token(line, current_pos, *line_nr, TOKEN_GREATER, TOKEN_GREATER_EQUAL, '=');
            break;
        case '<':
            *token = create_multi_char_token(line, current_pos, *line_nr, TOKEN_LESS, TOKEN_LESS_EQUAL, '=');
            break;

        case '/': {
            // strip out comments
            if(match_next(line, current_pos, "/")) {
                advance_until(line, current_pos, "\n\0", 0);
                return 0;
            }
            else {
                *token = create_token(TOKEN_SLASH, current_char, 1, *line_nr);
                break; 
            }
        }
        case ' ':
        case '\r':
        case '\t':
            return 0;

        case '\n':
            *line_nr = *line_nr + 1;
            return 0;

        default:
            if(is_number_candidate(*current_char)) {
                char* out_num = NULL;
                //
                // backtrack because first number cancidate char check already consumed position 
                // and position starting with initial one is needed to resolve full number
                *current_pos = *current_pos - 1;

                int is_number_resolved = resolve_number(line, current_pos, &out_num);

                if(!is_number_resolved) return 0;

                size_t len = strlen(out_num);
                *token = create_token(TOKEN_NUMBER, out_num, len, *line_nr);
            } else if(*current_char == '\"') {
                size_t start = *current_pos;
                char* string_value = resolve_string(line, current_pos);

                if(!string_value) return 0;

                size_t len = *current_pos - start;

                *token = create_token(TOKEN_STRING, string_value, len, *line_nr);
            } else {
                *token = create_token(TOKEN_UNRECOGNIZED, current_char, 1, *line_nr);
            }
    }

    return 1;
}


void scan_tokens(char *line, size_t* line_nr,  TokenBuffer* token_buffer, size_t *current_pos) {
    Token token;

    do {
        int token_produced = scan_next_token(line, current_pos, line_nr, &token);

        if(!token_produced) continue;

        if(token.type == TOKEN_UNRECOGNIZED) {
            printf("unexpected token %s", token.lexeme);
            break;
        }

        add_token(token_buffer, token);
    } while(!line_is_at_end(line, *current_pos));
}
