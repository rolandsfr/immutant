#include <stddef.h>
#include "lexer.h"
#include <stdio.h>

#include "error.h"

void interpret(const char* source, size_t length) {
    // array to hold tokens scanned from source
    char tokens[256];
    scan_tokens(source, length, tokens);
    // error(1, "Interpretation not yet implemented.");
}
