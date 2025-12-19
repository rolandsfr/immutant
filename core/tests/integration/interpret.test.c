#include "interpret.h"

#include <stddef.h>

#include "unity.h"

#include "assert_value.h"
#include "ast_cnstrct.h"
#include "ast_expr.h"
#include "ast_make_expr.h"
#include "ast_make_stmt.h"
#include "env.h"
#include "error.h"
#include "error_codes.h"
#include "error_report.h"
#include "eval.h"
#include "eval_asgn.h"
#include "eval_binary.h"
#include "eval_block_stmt.h"
#include "eval_expr.h"
#include "eval_if_stmt.h"
#include "eval_logic.h"
#include "eval_unary.h"
#include "eval_var.h"
#include "eval_while_stmt.h"
#include "is_equal.h"
#include "lexer.h"
#include "make_values.h"
#include "parse_and.h"
#include "parse_asgn.h"
#include "parse_block_stmt.h"
#include "parse_comparison.h"
#include "parse_dclr.h"
#include "parse_eq.h"
#include "parse_expr.h"
#include "parse_expr_stmt.h"
#include "parse_factor.h"
#include "parse_lassoc.h"
#include "parse_or.h"
#include "parse_primary.h"
#include "parse_term.h"
#include "parse_unary.h"
#include "parse_var_dclr.h"
#include "parse_while_stmt.h"
#include "parser.h"
#include "parser_helpers.h"
#include "parser_singnature.h"
#include "require_t.h"
#include "resolve.h"
#include "sync_parse.h"
#include "test_expr.h"
#include "value_t.h"

#include "Mockparse_if_stmt.h"

void test_interpret_source()
{
	size_t line_nr = 0;
	Values result = interpret_source("1 + 1;2+2;", 6, &line_nr);
	TEST_ASSERT_NUMBER_VALUE(result.items[0], 2);
	TEST_ASSERT_NUMBER_VALUE(result.items[1], 4);
}

void test_interpret_source_complex_expression()
{
	size_t line_nr = 0;
	Values result = interpret_source("12 / 4 + (3 + -5);", 6, &line_nr);
	TEST_ASSERT_NUMBER_VALUE(result.items[0], 1);
}

void test_return_on_missing_semicolon()
{
	size_t line_nr = 0;
	Values result = interpret_source("!false", 11, &line_nr);

	TEST_ASSERT_EQUAL_INT(0, result.count);
}

void test_interpret_source_string_concatenation()
{
	size_t line_nr = 0;
	Values result = interpret_source("\"Hello, \" + \"world!\";", 21, &line_nr);
	TEST_ASSERT_STRING_VALUE(result.items[0], "Hello, world!");
}

void test_interpret_unary_negation()
{
	size_t line_nr = 0;
	Values result = interpret_source("-42;", 3, &line_nr);
	TEST_ASSERT_NUMBER_VALUE(result.items[0], -42);
}

void test_large_number_addition()
{
	size_t line_nr = 0;
	Values result = interpret_source("93499 + 14439.5;", 15, &line_nr);
	TEST_ASSERT_NUMBER_VALUE(result.items[0], 107938.5);
}

void test_interpret_var_usage()
{
	size_t line_nr = 0;
	interpret_source("immutant a = 10;", 15, &line_nr);
	interpret_source("immutant b = 5;", 14, &line_nr);
	Values result = interpret_source("a + b;", 21, &line_nr);
	TEST_ASSERT_NUMBER_VALUE(result.items[0], 15);
}
