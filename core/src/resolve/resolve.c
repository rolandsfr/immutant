#include "resolve.h"

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "lexer.h"

/** string resolver */

char* resolve_string(char* line, size_t* pos)
{
	size_t start = *pos;

	const char* advanced_char = advance_until(line, pos, "\"", 1);
	if (!advanced_char)
		return NULL;

	size_t len = *pos - start;
	char* res = malloc(len + 1);

	if (!res)
		return NULL;

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

int resolve_number(char* line, size_t* pos, char** out_num)
{
	size_t start = *pos;

	while (isdigit(*peek(line, pos))) {
		advance(line, pos);
	}

	if (*peek(line, pos) == '.' && isdigit(*peek_next(line, *pos))) {
		advance(line, pos);
		while (isdigit(*peek(line, pos))) {
			advance(line, pos);
		}
	}

	size_t len = *pos - start;

	if (len == 0)
		return 0;

	*out_num = malloc(len + 1);

	if (!*out_num)
		return 0;

	memcpy(*out_num, line + start, len + 1);
	(*out_num)[len] = '\0';

	return 1;
}

int resolve_identifier(char* line, size_t* pos, char** out_identifier)
{
	size_t start = *pos;

	while (isalnum(*peek(line, pos)) || *peek(line, pos) == '_') {
		advance(line, pos);
	}

	size_t len = *pos - start;

	if (len == 0)
		return 0;

	*out_identifier = malloc(len + 1);

	if (!*out_identifier)
		return 0;

	memcpy(*out_identifier, line + start, len);
	(*out_identifier)[len] = '\0';

	return 1;
}

int resolve_keyword(char* line, size_t pos, size_t len,
					enum TokenType* token_out)
{
	char* lexeme = line + pos;

	// check first letter first to reduce number of character comparisons later
	// caused by strcmp additionally short circuit on length check
	switch (lexeme[0]) {
		case 'e':
			if (len == 4 && strcmp(lexeme, "else") == 0) {
				*token_out = TOKEN_ELSE;
				return 1;
			}
			break;
		case 'i':
			if (len == 2 && strcmp(lexeme, "if") == 0) {
				*token_out = TOKEN_IF;
				return 1;
			} else if (len == 6 && strcmp(lexeme, "impure") == 0) {
				*token_out = TOKEN_IMPURE;
				return 1;
			} else if (len == 8 && strcmp(lexeme, "immutant") == 0) {
				*token_out = TOKEN_IMMUTANT;
				return 1;
			}
			break;
		case 'm':
			if (len == 6 && strcmp(lexeme, "mutant") == 0) {
				*token_out = TOKEN_MUTANT;
				return 1;
			}
			break;
		case 'f':
			if (len == 2 && strcmp(lexeme, "fn") == 0) {
				*token_out = TOKEN_FN;
				return 1;
			} else if (len == 5 && strcmp(lexeme, "false") == 0) {
				*token_out = TOKEN_FALSE;
				return 1;
			}
			break;
		case 'p':
			if (len == 4 && strcmp(lexeme, "pure") == 0) {
				*token_out = TOKEN_PURE;
				return 1;
			}
			break;
		case 'r':
			if (len == 6 && strcmp(lexeme, "return") == 0) {
				*token_out = TOKEN_RETURN;
				return 1;
			}
			break;
		case 't':
			if (len == 4 && strcmp(lexeme, "true") == 0) {
				*token_out = TOKEN_TRUE;
				return 1;
			}
			break;
		case 'w':
			if (len == 5 && strcmp(lexeme, "while") == 0) {
				*token_out = TOKEN_WHILE;
				return 1;
			}
			break;
	}

	return -1;
}
