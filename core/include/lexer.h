#ifndef LEXER_H
#define LEXER_H

#include <stddef.h>

enum TokenType
{
    // Single-character tokens.
    TOKEN_LEFT_PAREN,
    TOKEN_RIGHT_PAREN,
    TOKEN_LEFT_BRACE,
    TOKEN_RIGHT_BRACE,
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_STAR,
    TOKEN_SLASH,
    TOKEN_SEMICOLON,
    TOKEN_DOT,

    // Comparison / logical tokens and assignment.
    TOKEN_BANG,
    TOKEN_BANG_EQUAL,
    TOKEN_EQUAL,
    TOKEN_EQUAL_EQUAL,
    TOKEN_GREATER,
    TOKEN_GREATER_EQUAL,
    TOKEN_LESS,
    TOKEN_LESS_EQUAL,
    TOKEN_AND,
    TOKEN_OR,

    // Literals
    TOKEN_IDENTIFIER,
    TOKEN_STRING,
    TOKEN_NUMBER,

    // Keywords
    TOKEN_FALSE,
    TOKEN_TRUE,
    TOKEN_NIL,
    TOKEN_WHILE,
    TOKEN_IF,
    TOKEN_ELSE,
    TOKEN_IMMUTANT,
    TOKEN_MUTANT,
    TOKEN_FN,
    TOKEN_RETURN,

    // End of file
    TOKEN_EOF,

    // Not supported - not part of syntax
    TOKEN_UNRECOGNIZED
};

typedef struct
{
    enum TokenType type;
    const char *lexeme;
    size_t length;
    int line;
} Token;

typedef struct
{
    Token *tokens;
    size_t count;
    size_t capacity;
} TokenBuffer;

/**
 * Token management
 * ---------------- */

/** Returns an initialized token struct */
Token create_token(enum TokenType type, const char *lexeme, size_t length, int line);

/** Pushes token to allocated buffer */
void add_token(TokenBuffer *buffer, Token token);

/**  initializes struct with non-null data and allocates buffer within it */
void init_token_buffer(TokenBuffer *buffer);

/**  frees allocated buffer within struct */
void free_token_buffer(TokenBuffer *buffer);

/** --------------- */

/**
 * Lexing - identifying tokens
 * ---------------------------
 **/

void scan_tokens(char *line, int line_nr, TokenBuffer* token_buffer, size_t *current_pos);

int scan_next_token(char *line, size_t *current_pos, int line_nr, Token* token);

/** Base unit function for identifying closest lexeme from current position  */
void lex_single_token(char *line, int line_nr, size_t length, TokenBuffer *buffer, size_t *current_pos);

/** Manipulates start and current position in source line to get all tokens in it*/
void lex_line_tokens(char *line, size_t length, TokenBuffer *buffer, size_t start_pos, size_t current_pos);

/** Scans source code and accumulates tokens line by line */
void scan_file_tokens(char *line, size_t length, TokenBuffer *buffer);

/** looks up ahead 1 char from current position without consuming the character */
const char *peek(char *line, size_t *current_pos);

/* returns character at the current position and advances the current position by one */
const char *advance(char *line, size_t *current_pos);

int line_is_at_end(char *line, size_t current_pos);

/**
 * Advances position if character at current position matches expected one.
 * @return number boolean indicating whether expected character was or was not matched */
int match_next(char *line, size_t *current_pos, const char* expected);

/** --------------------------- */

#endif // LEXER_H
