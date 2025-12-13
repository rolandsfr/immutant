#pragma once

#define TEST_ASSERT_VAR_STMT(STMT, EXPECTED_MUT, EXPECTED_NAME)                \
	do {                                                                       \
		TEST_ASSERT_NOT_NULL(STMT);                                            \
		TEST_ASSERT_EQUAL_INT(STMT_VAR_DECL, (STMT)->base.type);               \
		TEST_ASSERT_EQUAL_INT(EXPECTED_MUT, (STMT)->mutability);               \
		TEST_ASSERT_EQUAL_STRING(EXPECTED_NAME, (STMT)->name);                 \
	} while (0)

#define TEST_ASSERT_VAR_STMT_INIT(STMT, EXPECTED_MUT, EXPECTED_NAME,           \
								  EXPECTED_INIT_VALUE)                         \
	do {                                                                       \
		TEST_ASSERT_VAR_STMT(STMT, EXPECTED_MUT, EXPECTED_NAME);               \
		TEST_ASSERT_NOT_NULL((STMT)->initializer);                             \
		/* Assuming initializer is a number literal for simplicity */          \
		NumberExpr* num_expr = (NumberExpr*)((STMT)->initializer);             \
		TEST_ASSERT_EQUAL_STRING(EXPECTED_INIT_VALUE, num_expr->value);        \
	} while (0)
