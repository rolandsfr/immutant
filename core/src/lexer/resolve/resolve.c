#include <string.h>
#include <stdlib.h>

#include "resolve.h"
#include "lexer.h"

char* resolve_string(char* line, size_t* pos)
{
    size_t start = *pos;
    const char *peeked;

    // match until eof or matching enclosing string character
    while((peeked = peek(line, pos)) != NULL && *peeked != '"') {
        printf("peeked char: %c\n", *peeked);
        advance(line, pos);
    }

    size_t len = *pos - start;
    char* res = malloc(len + 1);

    if(!res) return NULL;

    memcpy(res, line + start, len);
    res[len] = '\0';

    if (peeked && *peeked == '"') {
        advance(line, pos);
    }

    return res;
}

