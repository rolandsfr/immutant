#include "sync_parse.h"

#include <stdio.h>

#include "error.h"
#include "lexer.h"
#include "parser_helpers.h"
#include "stddef.h"

void sync_parse(TokenBuffer* tokens, size_t* pos, Error* out_error)
{
	consume_token(tokens, pos); // consume error token

	if (out_error) {
		*out_error = (Error){.type = ERROR_NONE, .message = ""};
	}

	while (!is_at_end(tokens, *pos)) {
		enum TokenType next_token = peek_token(tokens, *pos);

		switch (next_token) {
			case TOKEN_FN:
			case TOKEN_MUTANT:
			case TOKEN_IMMUTANT:
			case TOKEN_IF:
			case TOKEN_WHILE:
			case TOKEN_RETURN: {

				// reset error
				return; // Found a statement start, exit synchronization
			}
		}

		consume_token(tokens, pos); // consume token and continue
	}
}
