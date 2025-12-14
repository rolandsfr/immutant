#include "interpret.h"

#include <stddef.h>

#include "ast_expr.h"
#include "ast_stmt.h"
#include "error.h"
#include "error_codes.h"
#include "error_report.h"
#include "eval.h"
#include "lexer.h"
#include "make_values.h"
#include "parse_eq.h"
#include "parse_expr_stmt.h"
#include "parser.h"
#include "value_t.h"

void init_value_array(Values* arr)
{
	arr->count = 0;
	arr->capacity = 8;
	arr->items = malloc(arr->capacity * sizeof(Value));
}

void add_value(Values* arr, Value v)
{
	if (arr->count >= arr->capacity) {
		arr->capacity *= 2;
		arr->items = realloc(arr->items, arr->capacity * sizeof(Value));
	}
	arr->items[arr->count++] = v;
}

Values interpret_source(char* source, size_t length, size_t* line_nr)
{
	Values results;
	init_value_array(&results);

	TokenBuffer token_buffer;
	init_token_buffer(&token_buffer);

	size_t cursor = 0;

	ErrorBuffer lex_error_buffer;
	ErrorBuffer_init(&lex_error_buffer);
	scan_tokens(source, line_nr, &token_buffer, &cursor, &lex_error_buffer);

	if (had_errors(&lex_error_buffer)) {
		report_errors(&lex_error_buffer);
		free_token_buffer(&token_buffer);
		ErrorBuffer_free(&lex_error_buffer);
		return results;
	}

	ErrorBuffer errors;
	ErrorBuffer_init(&errors);
	Stmts stmts = parse(&token_buffer, &errors);

	if (had_errors(&errors)) {
		report_errors(&errors);
		ErrorBuffer_free(&errors);
		return results;
	}

	for (size_t i = 0; i < stmts.count; i++) {
		Error runtime_error = {-1};
		ExprStmt* expr_stmt = stmts.expr_stmts[i];
		Value value = eval_expr(expr_stmt->expression, &runtime_error);

		if (runtime_error.type != ERROR_NONE) {
			printf("Runtime error at line %zu: %s\n", *line_nr,
				   runtime_error.message);
			free_token_buffer(&token_buffer);
			int hadError = 1;
			report_error(*line_nr, runtime_error.message, &hadError);
			continue;
		}

		add_value(&results, value);
	}

	return results;
}
