#ifndef RESOLVE_H
#define RESOLVE_H

#include <stdio.h>

/**
 * Resolving values of different kinds 
 * ---------------- */

/** Resolves string value */
char* resolve_string(char* line, size_t* pos);

/** Resolves number value as a string (both integers and numbers with decimals) */
int resolve_number(char *line, size_t *pos, char** out_num);

/** returns boolean indicating if starting from pos in the given line value can be resolved as a number */
int is_number_candidate(const char ch);

/** --------------------------- */
#endif
