#include "interpret.h"

#include <stddef.h>

#include "ast_expr.h"
#include "error.h"
#include "error_report.h"
#include "eval.h"
#include "lexer.h"
#include "make_runtime_err.h"
#include "make_values.h"
#include "parse_eq.h"
#include "runtime_err.h"
#include "value_t.h"

Value interpret_source(char* source, size_t length, size_t* line_nr)
{
	TokenBuffer token_buffer;
	init_token_buffer(&token_buffer);

	size_t cursor = 0;

	scan_tokens(source, line_nr, &token_buffer, &cursor, NULL);

	// resetting cursor back to 0 to reuse it in parsing cursor = 0;
	ErrorReport error_report = init_no_error_report();

	cursor = 0;
	Expr* expr = parse_equality(&token_buffer, &cursor, &error_report);
	if (error_report.code != NO_ERROR) {
		// Handle error (for simplicity, returning a default Value here)
		free_token_buffer(&token_buffer);
		int hadError = 1;
		report_error(*line_nr, error_report.message, &hadError);
		return make_null();
	}

	RuntimeError runtime_error = make_runtime_error(RUNTIME_NO_ERROR, "");
	Value result = eval_expr(expr, &runtime_error);

	if (runtime_error.type != RUNTIME_NO_ERROR) {
		// Handle runtime error (for simplicity, returning a default Value here)
		free_token_buffer(&token_buffer);
		int hadError = 1;
		report_error(*line_nr, runtime_error.message, &hadError);
		return make_null();
	}

	free_token_buffer(&token_buffer);

	return result;
}
