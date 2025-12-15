#include "parser.h"

#include "array.h"
#include "ast_make_expr.h"
#include "ast_make_stmt.h"
#include "ast_stmt.h"
#include "error.h"
#include "lexer.h"
#include "parser_helpers.h"
#include "resolve.h"
#include "sync_parse.h"
#include "test_expr.h"

#include "Mockparse_dclr.h"

Stmt* mock_dclr_and_throw(TokenBuffer* tokens, size_t* pos, Error* out_error,
						  int num_calls)
{
	if (num_calls == 0) {

		if (out_error) {
			*out_error = (Error){
				.type = SYNTAX_ERROR_UNEXPECTED_TOKEN,
				.message = "Expected ';' after variable declaration.",
			};
		}
		*pos = 3;
		return NULL;
	}

	Expr* expr = (Expr*)make_number_expr("42");
	*pos += 1;
	return (Stmt*)make_expr_stmt(expr);
}

void test_parse_var_decl_should_error_if_no_semicolon_after_initializer()
{
	TokenBuffer tokens;
	init_token_buffer(&tokens);
	size_t pos = 0;
	ErrorBuffer errors;
	ErrorBuffer_init(&errors);

	parse_dclr_StubWithCallback(mock_dclr_and_throw);

	add_token(&tokens, create_token(TOKEN_MUTANT, "mutant", 6, 1));
	add_token(&tokens, create_token(TOKEN_IDENTIFIER, "x", 1, 1));
	add_token(&tokens, create_token(TOKEN_EQUAL, "=", 1, 1));
	add_token(&tokens, create_token(TOKEN_NUMBER, "42", 2, 1));
	add_token(&tokens, create_token(TOKEN_IF, "if", 2,
									1)); // to be re-started from

	Stmts stmts = parse(&tokens, &errors);

	TEST_ASSERT_EQUAL_INT(1, stmts.len);
	TEST_ASSERT_EQUAL_INT(1, errors.len);
	TEST_ASSERT_EQUAL_INT(SYNTAX_ERROR_UNEXPECTED_TOKEN, errors.data[0].type);
	ExprStmt* expr_stmt = (ExprStmt*)(stmts.data[0]);
	TEST_ASSERT_LITERAL_NUMBER_EXPR(expr_stmt->expression, "42");
}
