#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "lexer.h"
#include "error.h"


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
    case '/': {
		      // strip out comments
		      if(match_next(line, current_pos, "/")) {
			      const char* peeked_char = peek(line, current_pos);
			      while(peeked_char != NULL && *peeked_char != '\n' && *peeked_char != '\0') {
				      advance(line, current_pos);
				      peeked_char = peek(line, current_pos);
			      }
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
	*token = create_token(TOKEN_UNRECOGNIZED, current_char, 1, *line_nr);
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
	}

	while(!line_is_at_end(line, *current_pos));

}
