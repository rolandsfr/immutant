#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "resolve.h"
#include "lexer.h"


/** string resolver */
char* resolve_string(char* line, size_t* pos)
{
    size_t start = *pos;

    advance_until(line, pos, "\"", 1);

    size_t len = *pos - start;
    char* res = malloc(len + 1);

    if(!res) return NULL;

    memcpy(res, line + start, len - 1); // len - 1 to ignore last consumed "
    res[len] = '\0';

    return res;
}

/** number resolver */

void resolve_number(char* line, size_t *pos)
{
    size_t start = *pos;

    while(isdigit(*line + *pos)) advance(line, pos);
    


}
