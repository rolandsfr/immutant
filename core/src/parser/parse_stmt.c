#include "parse_stmt.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "array.h"
#include "ast_make_stmt.h"
#include "error.h"
#include "lexer.h"
#include "parse_expr.h"
#include "parser_helpers.h"
#include "value_t.h"

ARR_DEFINE(Stmt*, StmtBuffer);

static VarDeclStmt* parse_var_decl(TokenBuffer* tokens, size_t* pos,
								   enum MutabilityType mutability,
								   Error* out_error);
static BlockStmt* parse_block_stmt(TokenBuffer* tokens, size_t* pos,
								   Error* out_error);
static IfStmt* parse_if_stmt(TokenBuffer* tokens, size_t* pos,
							 Error* out_error);
static ReturnStmt* parse_return_stmt(TokenBuffer* tokens, size_t* pos,
									 Error* out_error);
static WhileStmt* parse_while_stmt(TokenBuffer* tokens, size_t* pos,
								   Error* out_error);
static FunDeclStmt* parse_fun_decl(TokenBuffer* tokens, size_t* pos,
								   Error* out_error);
static ExprStmt* parse_expr_stmt(TokenBuffer* tokens, size_t* pos,
								 Error* out_error);

Stmt* parse_dclr(TokenBuffer* tokens, size_t* pos, Error* out_error)
{
	if (match_token(tokens, pos, 2, TOKEN_MUTANT, TOKEN_IMMUTANT)) {
		enum MutabilityType mutability =
			previous_token(tokens, *pos) == TOKEN_MUTANT ? MUTABLE : IMMUTABLE;

		return (Stmt*)parse_var_decl(tokens, pos, mutability, out_error);
	}

	if (match_token(tokens, pos, 1, TOKEN_LEFT_BRACE)) {
		return (Stmt*)parse_block_stmt(tokens, pos, out_error);
	}

	if (match_token(tokens, pos, 1, TOKEN_IF)) {
		return (Stmt*)parse_if_stmt(tokens, pos, out_error);
	}

	if (match_token(tokens, pos, 1, TOKEN_RETURN)) {
		return (Stmt*)parse_return_stmt(tokens, pos, out_error);
	}

	if (match_token(tokens, pos, 1, TOKEN_WHILE)) {
		return (Stmt*)parse_while_stmt(tokens, pos, out_error);
	}

	if (match_token(tokens, pos, 3, TOKEN_FN, TOKEN_PURE, TOKEN_IMPURE)) {
		*pos = *pos - 1;
		return (Stmt*)parse_fun_decl(tokens, pos, out_error);
	}

	return (Stmt*)parse_expr_stmt(tokens, pos, out_error);
}

static VarDeclStmt* parse_var_decl(TokenBuffer* tokens, size_t* pos,
								   enum MutabilityType mutability,
								   Error* out_error)
{
	Token name = consume_token(tokens, pos);
	if (name.type != TOKEN_IDENTIFIER) {
		if (out_error) {
			*out_error = (Error){.type = SYNTAX_ERROR_UNEXPECTED_TOKEN,
								 .line = name.line,
								 .message = "Expected variable name"};
		}
		return NULL;
	}
	Expr* initializer = NULL;

	if (match_token(tokens, pos, 1, TOKEN_EQUAL)) {
		initializer = parse_expr(tokens, pos, out_error);
		if (out_error && out_error->type != ERROR_NONE) {
			return NULL;
		}
	} else if (mutability == IMMUTABLE) {
		if (out_error) {
			*out_error =
				(Error){.type = SYNTAX_ERROR_UNEXPECTED_TOKEN,
						.message = "Immutable variables must be initialized.",
						.line = name.line};
		}
		return NULL;
	}

	if (!match_token(tokens, pos, 1, TOKEN_SEMICOLON)) {
		if (out_error) {
			*out_error =
				(Error){.type = SYNTAX_ERROR_UNEXPECTED_TOKEN,
						.message = "Expected ';' after variable declaration.",
						.line = peek_token_full(tokens, *pos).line};
		}

		free_expr(initializer);
		return NULL;
	}

	return make_var_decl_stmt(name.lexeme, initializer, mutability, name.line);
}

static BlockStmt* parse_block_stmt(TokenBuffer* tokens, size_t* pos,
								   Error* out_error)
{
	StmtBuffer stmt_buffer;
	StmtBuffer_init(&stmt_buffer);

	while (!check_token(tokens, *pos, TOKEN_RIGHT_BRACE) &&
		   !is_at_end(tokens, *pos)) {

		Stmt* stmt = parse_dclr(tokens, pos, out_error);

		if ((out_error && out_error->type != ERROR_NONE)) {
			StmtBuffer_free(&stmt_buffer);
			return NULL;
		}

		StmtBuffer_push(&stmt_buffer, stmt);
	}

	Token block_token = peek_token_full(tokens, *pos - 1);
	if (!match_token(tokens, pos, 1, TOKEN_RIGHT_BRACE)) {
		if (out_error) {
			*out_error = (Error){.type = SYNTAX_ERROR_UNEXPECTED_TOKEN,
								 .line = peek_token_full(tokens, *pos).line,
								 .message = ""};
			snprintf(out_error->message, sizeof(out_error->message),
					 "Expected '}' after block statement");
		}

		StmtBuffer_free(&stmt_buffer);
		return NULL;
	}

	return make_block_stmt(stmt_buffer.data, stmt_buffer.len, block_token.line);
}

static IfStmt* parse_if_stmt(TokenBuffer* tokens, size_t* pos, Error* out_error)
{
	if (!match_token(tokens, pos, 1, TOKEN_LEFT_PAREN)) {
		if (out_error) {
			*out_error = (Error){.type = SYNTAX_ERROR_UNEXPECTED_TOKEN,
								 .line = peek_token_full(tokens, *pos).line,
								 .message = "Expected '(' after 'if'"};
		}
		return NULL;
	}

	Expr* condition = parse_expr(tokens, pos, out_error);

	if (!condition) {
		if (out_error) {
			*out_error =
				(Error){.type = SYNTAX_ERROR_UNEXPECTED_TOKEN,
						.line = peek_token_full(tokens, *pos).line,
						.message = "Invalid expression in if condition"};
		}
		return NULL;
	}

	if (!match_token(tokens, pos, 1, TOKEN_RIGHT_PAREN)) {
		if (out_error) {
			*out_error = (Error){.type = SYNTAX_ERROR_UNEXPECTED_TOKEN,
								 .line = peek_token_full(tokens, *pos).line,
								 .message = "Expected ')' after expression"};
		}
		return NULL;
	}

	Stmt* then_branch = parse_dclr(tokens, pos, out_error);

	Stmt* else_branch = NULL;
	if (match_token(tokens, pos, 1, TOKEN_ELSE)) {
		else_branch = parse_dclr(tokens, pos, out_error);
	}

	return make_if_stmt(condition, then_branch, else_branch, condition->line);
}

static ReturnStmt* parse_return_stmt(TokenBuffer* tokens, size_t* pos,
									 Error* out_error)
{
	Expr* value = NULL;

	if (!check_token(tokens, *pos, TOKEN_SEMICOLON)) {
		value = parse_expr(tokens, pos, out_error);

		if (out_error && out_error->type != ERROR_NONE) {
			return NULL;
		}
	}

	if (!match_token(tokens, pos, 1, TOKEN_SEMICOLON)) {
		if (out_error) {
			*out_error =
				(Error){.type = SYNTAX_ERROR_UNEXPECTED_TOKEN,
						.line = peek_token_full(tokens, *pos).line,
						.message = "Expected ';' after return statement"};
		}

		free_expr(value);
		return NULL;
	}

	Token return_token = peek_token_full(tokens, *pos - 1);
	return make_return_stmt(value, return_token.line);
}

static WhileStmt* parse_while_stmt(TokenBuffer* tokens, size_t* pos,
								   Error* out_error)
{
	if (!match_token(tokens, pos, 1, TOKEN_LEFT_PAREN)) {
		if (out_error) {
			*out_error = (Error){.type = SYNTAX_ERROR_UNEXPECTED_TOKEN,
								 .line = peek_token_full(tokens, *pos).line,
								 .message = "Expected '(' after 'while'"};
		}
		return NULL;
	}

	Expr* condition = parse_expr(tokens, pos, out_error);

	if (!condition) {
		if (out_error) {
			*out_error =
				(Error){.type = SYNTAX_ERROR_UNEXPECTED_TOKEN,
						.line = peek_token_full(tokens, *pos).line,
						.message = "Invalid expression in while condition"};
		}
		return NULL;
	}

	if (!match_token(tokens, pos, 1, TOKEN_RIGHT_PAREN)) {
		if (out_error) {
			*out_error = (Error){.type = SYNTAX_ERROR_UNEXPECTED_TOKEN,
								 .line = peek_token_full(tokens, *pos).line,
								 .message = "Expected ')' after expression"};
		}
		return NULL;
	}

	Stmt* body_stmt = parse_dclr(tokens, pos, out_error);

	return make_while_stmt(condition, body_stmt, condition->line);
}

static FunDeclStmt* parse_fun_decl(TokenBuffer* tokens, size_t* pos,
								   Error* out_error)
{
	PurityType purity = PURE;
	Token first_token = consume_token(tokens, pos);

	if (first_token.type == TOKEN_PURE) {
		purity = PURE;
		if (!match_token(tokens, pos, 1, TOKEN_FN)) {
			if (out_error) {
				*out_error = (Error){.type = SYNTAX_ERROR_UNEXPECTED_TOKEN,
									 .line = peek_token_full(tokens, *pos).line,
									 .message = "Expected 'fn' after 'pure'"};
			}
			return NULL;
		}
	} else if (first_token.type == TOKEN_IMPURE) {
		purity = IMPURE;
		if (!match_token(tokens, pos, 1, TOKEN_FN)) {
			if (out_error) {
				*out_error = (Error){.type = SYNTAX_ERROR_UNEXPECTED_TOKEN,
									 .line = peek_token_full(tokens, *pos).line,
									 .message = "Expected 'fn' after 'impure'"};
			}
			return NULL;
		}
	} else if (first_token.type != TOKEN_FN) {
		if (out_error) {
			*out_error =
				(Error){.type = SYNTAX_ERROR_UNEXPECTED_TOKEN,
						.line = first_token.line,
						.message = "Expected 'fn', 'pure fn', or 'impure fn'"};
		}
		return NULL;
	}

	Token name = consume_token(tokens, pos);
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

	return make_fun_decl_stmt(name.lexeme, params, params->len, body, purity,
							  name.line);
}

static ExprStmt* parse_expr_stmt(TokenBuffer* tokens, size_t* pos,
								 Error* out_error)
{
	Expr* expr = parse_expr(tokens, pos, out_error);
	if (out_error && out_error->type != ERROR_NONE || expr == NULL) {
		return NULL;
	}

	if (!match_token(tokens, pos, 1, TOKEN_SEMICOLON)) {
		if (out_error) {
			Token token = peek_token_full(tokens, *pos);
			*out_error =
				(Error){.type = SYNTAX_ERROR_UNEXPECTED_TOKEN,
						.message = "Expected ';' after expression statement.",
						.line = token.line};
		}

		free_expr(expr);

		return NULL;
	}

	return make_expr_stmt(expr, expr->line);
}
