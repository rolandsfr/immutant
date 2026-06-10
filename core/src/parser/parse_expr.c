#include "parse_expr.h"

#include "array.h"
#include "ast_cnstrct.h"
#include "ast_make_expr.h"
#include "error_report.h"
#include "lexer.h"
#include "parser_helpers.h"
#include "parser_singnature.h"

ARR_DEFINE(Expr*, ArgumentsArray);

static Expr* parse_lassoc(TokenBuffer* tokens, size_t* pos,
						  ParseFn next_precedence_fn,
						  const enum TokenType* operators, size_t operator_count,
						  Error* out_error);
static Expr* parse_primary(TokenBuffer* tokens, size_t* pos, Error* out_error);
static Expr* parse_call(TokenBuffer* tokens, size_t* pos, Error* out_error);
static Expr* parse_unary(TokenBuffer* tokens, size_t* pos, Error* out_error);
static Expr* parse_factor(TokenBuffer* tokens, size_t* pos, Error* out_error);
static Expr* parse_term(TokenBuffer* tokens, size_t* pos, Error* out_error);
static Expr* parse_comparison(TokenBuffer* tokens, size_t* pos,
							  Error* out_error);
static Expr* parse_equality(TokenBuffer* tokens, size_t* pos, Error* out_error);
static Expr* parse_and(TokenBuffer* tokens, size_t* pos, Error* out_error);
static Expr* parse_or(TokenBuffer* tokens, size_t* pos, Error* out_error);
static Expr* parse_asgn(TokenBuffer* tokens, size_t* pos, Error* out_error);

Expr* parse_expr(TokenBuffer* tokens, size_t* pos, Error* out_error)
{
	return parse_asgn(tokens, pos, out_error);
}

static Expr* parse_lassoc(TokenBuffer* tokens, size_t* pos,
						  ParseFn next_precedence_fn,
						  const enum TokenType* operators, size_t operator_count,
						  Error* out_error)
{
	Expr* expr = next_precedence_fn(tokens, pos, out_error);

	if (out_error && out_error->type != ERROR_NONE) {
		return NULL;
	}

	while (match_any_token(tokens, pos, operators, operator_count)) {
		Token operator = tokens->tokens[*pos - 1];
		Expr* right = next_precedence_fn(tokens, pos, out_error);

		if (out_error && out_error->type != ERROR_NONE || right == NULL) {
			return NULL;
		}

		expr =
			(Expr*)make_binary_expr(expr, operator.type, right, operator.line);
	}

	return expr;
}

static Expr* parse_primary(TokenBuffer* tokens, size_t* pos, Error* out_error)
{
	if (match_token(tokens, pos, 1, TOKEN_STRING)) {
		return (Expr*)make_string_expr(prev_token(tokens, *pos).lexeme,
									   prev_token(tokens, *pos).line);
	}

	if (match_token(tokens, pos, 1, TOKEN_NUMBER)) {
		return (Expr*)make_number_expr(prev_token(tokens, *pos).lexeme,
									   prev_token(tokens, *pos).line);
	}

	if (match_token(tokens, pos, 1, TOKEN_TRUE)) {
		return (Expr*)make_boolean_expr(1, prev_token(tokens, *pos).line);
	}

	if (match_token(tokens, pos, 1, TOKEN_FALSE)) {
		return (Expr*)make_boolean_expr(0, prev_token(tokens, *pos).line);
	}

	if (match_token(tokens, pos, 1, TOKEN_IDENTIFIER)) {
		return (Expr*)make_variable_expr(prev_token(tokens, *pos).lexeme,
										 prev_token(tokens, *pos).line);
	}

	if (match_token(tokens, pos, 1, TOKEN_LEFT_PAREN)) {
		Expr* expr = parse_or(tokens, pos, out_error);
		if (out_error && out_error->type != ERROR_NONE) {
			return NULL;
		}

		int is_matched = match_token(tokens, pos, 1, TOKEN_RIGHT_PAREN);

		if (!is_matched) {
			if (out_error) {
				*out_error = (Error){.type = SYNTAX_ERROR_UNEXPECTED_TOKEN,
									 .message = "Expected ')' after expression",
									 .line = tokens->tokens[*pos].line};
			}
			return NULL;
		}

		return expr;
	}

	if (prev_token(tokens, *pos).type != TOKEN_LEFT_PAREN && out_error) {
		*out_error = (Error){.type = SYNTAX_ERROR_MISSING_EXPRESSION,
							 .message = "Expected expression",
							 .line = tokens->tokens[*pos - 1].line};
	}

	return NULL;
}

static Expr* parse_call(TokenBuffer* tokens, size_t* pos, Error* out_error)
{
	Expr* expr = parse_primary(tokens, pos, out_error);

	if (out_error && out_error->type != ERROR_NONE) {
		return NULL;
	}

	while (1) {
		if (match_token(tokens, pos, 1, TOKEN_LEFT_PAREN)) {
			ArgumentsArray arguments;
			ArgumentsArray_init(&arguments);

			if (!check_token(tokens, *pos, TOKEN_RIGHT_PAREN)) {
				do {
					if (check_token(tokens, *pos, TOKEN_RIGHT_PAREN)) {
						if (out_error) {
							*out_error =
								(Error){.type = SYNTAX_ERROR_UNEXPECTED_TOKEN,
										.line = peek_token_full(tokens, *pos)
												   .line,
										.message = "Expected expression after "
													"',' in argument list."};
						}
						for (size_t i = 0; i < arguments.len; i++) {
							free_expr(arguments.data[i]);
						}
						ArgumentsArray_free(&arguments);
						return NULL;
					}

					Expr* argument = parse_expr(tokens, pos, out_error);
					if (out_error && out_error->type != ERROR_NONE) {
						for (size_t i = 0; i < arguments.len; i++) {
							free_expr(arguments.data[i]);
						}
						ArgumentsArray_free(&arguments);
						return NULL;
					}
					ArgumentsArray_push(&arguments, argument);
				} while (match_token(tokens, pos, 1, TOKEN_COMMA) &&
						 !is_at_end(tokens, *pos));
			}

			Token paren = consume_token(tokens, pos);

			if (paren.type != TOKEN_RIGHT_PAREN) {
				if (out_error) {
					*out_error =
						(Error){.type = SYNTAX_ERROR_UNEXPECTED_TOKEN,
								.line = paren.line,
								.message = "Expect ')' after arguments."};
				}

				for (size_t i = 0; i < arguments.len; i++) {
					free_expr(arguments.data[i]);
				}

				ArgumentsArray_free(&arguments);
				return NULL;
			}

			return (Expr*)make_call_expr(expr, arguments.data, arguments.len,
										 paren.line);
		}

		return expr;
	}
}

static Expr* parse_unary(TokenBuffer* tokens, size_t* pos, Error* out_error)
{
	if (match_token(tokens, pos, 2, TOKEN_MINUS, TOKEN_BANG)) {
		Token operator_token = prev_token(tokens, *pos);
		Expr* right = parse_unary(tokens, pos, out_error);

		if (out_error && out_error->type != ERROR_NONE || right == NULL) {
			return NULL;
		}

		return (Expr*)make_unary_expr(operator_token.type, right,
									  operator_token.line);
	}

	return parse_call(tokens, pos, out_error);
}

static Expr* parse_factor(TokenBuffer* tokens, size_t* pos, Error* out_error)
{
	const enum TokenType operators[] = {TOKEN_SLASH, TOKEN_STAR, TOKEN_MODULO};
	const size_t operators_count = sizeof(operators) / sizeof(operators[0]);

	return parse_lassoc(tokens, pos, parse_unary, operators, operators_count,
						out_error);
}

static Expr* parse_term(TokenBuffer* tokens, size_t* pos, Error* out_error)
{
	const enum TokenType operators[] = {TOKEN_MINUS, TOKEN_PLUS};
	const size_t operators_count = sizeof(operators) / sizeof(operators[0]);

	return parse_lassoc(tokens, pos, parse_factor, operators, operators_count,
						out_error);
}

static Expr* parse_comparison(TokenBuffer* tokens, size_t* pos, Error* out_error)
{
	const enum TokenType operators[] = {TOKEN_GREATER_EQUAL, TOKEN_GREATER,
										TOKEN_LESS_EQUAL, TOKEN_LESS};
	const size_t operators_count = sizeof(operators) / sizeof(operators[0]);

	return parse_lassoc(tokens, pos, parse_term, operators, operators_count,
						out_error);
}

static Expr* parse_equality(TokenBuffer* tokens, size_t* pos, Error* out_error)
{
	const enum TokenType operators[] = {TOKEN_BANG_EQUAL, TOKEN_EQUAL_EQUAL};
	const size_t operators_count = sizeof(operators) / sizeof(operators[0]);

	return parse_lassoc(tokens, pos, parse_comparison, operators,
						operators_count, out_error);
}

static Expr* parse_and(TokenBuffer* tokens, size_t* pos, Error* out_error)
{
	Expr* expr = parse_equality(tokens, pos, out_error);

	while (match_token(tokens, pos, 1, TOKEN_AND)) {
		Token operator = prev_token(tokens, *pos);

		Expr* right = parse_equality(tokens, pos, out_error);

		if (out_error && out_error->type != ERROR_NONE || right == NULL) {
			return NULL;
		}

		expr =
			(Expr*)make_logical_expr(expr, operator.type, right, operator.line);
	}

	return expr;
}

static Expr* parse_or(TokenBuffer* tokens, size_t* pos, Error* out_error)
{
	Expr* expr = parse_and(tokens, pos, out_error);

	while (match_token(tokens, pos, 1, TOKEN_OR)) {
		Token operator = prev_token(tokens, *pos);

		Expr* right = parse_and(tokens, pos, out_error);

		if (out_error && out_error->type != ERROR_NONE || right == NULL) {
			return NULL;
		}

		expr =
			(Expr*)make_logical_expr(expr, operator.type, right, operator.line);
	}

	return expr;
}

static Expr* parse_asgn(TokenBuffer* tokens, size_t* pos, Error* out_error)
{
	Expr* expr = parse_or(tokens, pos, out_error);

	if (out_error && out_error->type != ERROR_NONE) {
		return NULL;
	}

	if (match_token(tokens, pos, 1, TOKEN_EQUAL)) {
		Token target = tokens->tokens[*pos - 2];
		Expr* val_expr = parse_asgn(tokens, pos, out_error);
		if (out_error && out_error->type != ERROR_NONE) {
			free_expr(expr);
			return NULL;
		}

		if (expr->type != EXPR_VARIABLE) {
			if (out_error) {
				*out_error = (Error){
					.type = SYNTAX_ERROR_INVALID_ASSIGNMENT_TARGET,
					.line = target.line,
					.message = "Invalid assignment target.",
				};
			}
			free_expr(val_expr);
			free_expr(expr);
			return NULL;
		}

		return (Expr*)make_assign_expr(target.lexeme, val_expr, target.line);
	}

	return expr;
}
