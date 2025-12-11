#include "interpret.h"

#include <stddef.h>

#include "unity.h"

#include "assert_value.h"
#include "ast_cnstrct.h"
#include "ast_expr.h"
#include "ast_make_expr.h"
#include "error.h"
#include "error_codes.h"
#include "error_report.h"
#include "eval.h"
#include "eval_binary.h"
#include "eval_unary.h"
#include "is_equal.h"
#include "lexer.h"
#include "lexer.h" // TODO: remove after decoupled
#include "lexer.h"
#include "make_values.h"
#include "parse_comparison.h"
#include "parse_eq.h"
#include "parse_factor.h"
#include "parse_lassoc.h"
#include "parse_primary.h"
#include "parse_term.h"
#include "parse_unary.h"
#include "parser_helpers.h"
#include "parser_singnature.h"
#include "require_t.h"
#include "resolve.h"
#include "resolve.h" // TODO: remove after decoupled
#include "resolve.h"
#include "test_expr.h"
#include "value_t.h"

void test_interpret_source()
{
	size_t line_nr = 0;
	Value result = interpret_source("1 + 1", 6, &line_nr);
	TEST_ASSERT_NUMBER_VALUE(result, 2);
}

void test_interpret_source_complex_expression()
{
	size_t line_nr = 0;
	Value result = interpret_source("12 / 4 + (3 + -5)", 6, &line_nr);
	TEST_ASSERT_NUMBER_VALUE(result, 1);
}

void test_interpret_source_string_concatenation()
{
	size_t line_nr = 0;
	Value result = interpret_source("\"Hello, \" + \"world!\"", 21, &line_nr);
	TEST_ASSERT_STRING_VALUE(result, "Hello, world!");
}

void test_interpret_unary_negation()
{
	size_t line_nr = 0;
	Value result = interpret_source("-42", 3, &line_nr);
	TEST_ASSERT_NUMBER_VALUE(result, -42);
}

void test_large_number_addition()
{
	size_t line_nr = 0;
	Value result = interpret_source("93499 + 14439.5", 15, &line_nr);
	TEST_ASSERT_NUMBER_VALUE(result, 107938.5);
}
