#include "parse_fun_dclr.h"

#include "array.h"
#include "ast_make_stmt.h"
#include "ast_stmt.h"
#include "error.h"
#include "lexer.h"
#include "parse_block_stmt.h"
#include "parse_dclr.h"
#include "parse_expr_stmt.h"
#include "parse_if_stmt.h"
#include "parse_var_dclr.h"
#include "parse_while_stmt.h"
#include "parser_helpers.h"
#include "value_t.h"

FunDeclStmt* parse_fun_decl(TokenBuffer* tokens, size_t* pos, Error* out_error)
{
	// consume function singnature

	Token purity_token_or_name = consume_token(tokens, pos);
	Token name = purity_token_or_name;

	// check for optional pure or impure token
	if (purity_token_or_name.type == TOKEN_PURE ||
		purity_token_or_name.type == TOKEN_IMPURE) {
		name = consume_token(tokens, pos); // consume to get the name token
	} else if (purity_token_or_name.type != TOKEN_IDENTIFIER) {
		if (out_error) {
			*out_error = (Error){
				.type = SYNTAX_ERROR_UNEXPECTED_TOKEN,
				.line = purity_token_or_name.line,
				.message = "Expected function name or purity specifier"};
		}

		return NULL;
	}

	if (name.type != TOKEN_IDENTIFIER) {
		if (out_error) {
			*out_error = (Error){.type = SYNTAX_ERROR_UNEXPECTED_TOKEN,
								 .line = name.line,
								 .message = "Expected function name"};
		}
		return NULL;
	}

	if (!match_token(tokens, pos, 1, TOKEN_LEFT_PAREN)) {
		if (out_error) {
			*out_error = (Error){.type = SYNTAX_ERROR_UNEXPECTED_TOKEN,
								 .line = peek_token_full(tokens, *pos).line,
								 .message = "Expected '(' after function name"};
		}
		return NULL;
	}

	ParamArray* params = malloc(sizeof(ParamArray));
	ParamArray_init(params);

	if (!check_token(tokens, *pos, TOKEN_RIGHT_PAREN)) {
		do {
			Token param_name = consume_token(tokens, pos);
			if (param_name.type != TOKEN_IDENTIFIER) {
				if (out_error) {
					*out_error = (Error){.type = SYNTAX_ERROR_UNEXPECTED_TOKEN,
										 .line = param_name.line,
										 .message = "Expected parameter name"};
				}

				ParamArray_free(params);
				return NULL;
			}
			ParamArray_push(params, strdup(param_name.lexeme));
		} while (match_token(tokens, pos, 1, TOKEN_COMMA) &&
				 !is_at_end(tokens, *pos));
	}

	if (!match_token(tokens, pos, 1, TOKEN_RIGHT_PAREN)) {
		if (out_error) {
			*out_error =
				(Error){.type = SYNTAX_ERROR_UNEXPECTED_TOKEN,
						.line = peek_token_full(tokens, *pos).line,
						.message = "Expected ')' after function parameters"};
		}

		ParamArray_free(params);
		return NULL;
	}

	// consume function body

	Token left_brace = consume_token(tokens, pos);
	if (left_brace.type != TOKEN_LEFT_BRACE) {
		if (out_error) {
			*out_error =
				(Error){.type = SYNTAX_ERROR_UNEXPECTED_TOKEN,
						.line = left_brace.line,
						.message = "Expected '{' after function signature."};
		}

		ParamArray_free(params);
		return NULL;
	}

	BlockStmt* body = parse_block_stmt(tokens, pos, out_error);

	if (out_error && out_error->type != ERROR_NONE) {
		ParamArray_free(params);
		return NULL;
	}

	return make_fun_decl_stmt(
		name.lexeme, params, params->len, body,
		purity_token_or_name.type == TOKEN_IMPURE ? IMPURE : PURE, name.line);
}
