#include <string.h>
#include "utils.h"

/** Util for copying char by value */
const char *copy_char(const char *c)
{
    if (c == NULL)
        return NULL;

    static char copy[2];
    copy[0] = *c;
    copy[1] = '\0';
    return copy;
}
