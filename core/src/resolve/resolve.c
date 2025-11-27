#include <_string.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "resolve.h"
#include "lexer.h"



/** string resolver */

char* resolve_string(char* line, size_t* pos)
{
    size_t start = *pos;

    const char* advanced_char = advance_until(line, pos, "\"", 1);
    if(!advanced_char) return NULL;

    size_t len = *pos - start;
    char* res = malloc(len + 1);

    if(!res) return NULL;

    memcpy(res, line + start, len - 1); // len - 1 to ignore last consumed "
    res[len] = '\0';

    return res;
}

/** number resolver */

int is_number_candidate(const char ch)
{
    if (!(isdigit(ch) || ch == '.')) {
        return 0;
    }

    return 1;
}

int resolve_number(char *line, size_t *pos, char** out_num)
{
    size_t start = *pos;

    while(isdigit(*peek(line, pos))) {
        advance(line, pos);
    }

    if(*peek(line, pos) == '.' && isdigit(*peek_next(line, *pos))) {
        advance(line, pos);
        while(isdigit(*peek(line, pos))) {
            advance(line, pos);
        }
    }

    size_t len = *pos - start;

    if(len == 0) return 0;

    *out_num = malloc(len + 1);

    if(!*out_num) return 0;

    memcpy(*out_num, line + start, len + 1);
    (*out_num)[len] = '\0';

    return 1;
}

