#include "unity.h"
#include "lexer.h"

void setUp(void) {}
void tearDown(void) {}

void test_Multiplication_should_MultiplyRealNumbers(void)
{
  TEST_ASSERT_EQUAL_INT(1, multiply(1, 1));
  TEST_ASSERT_EQUAL_INT(6, multiply(3, 2));
  TEST_ASSERT_EQUAL_INT(27, multiply(3, 9));
}

void test_Multiplication_should_MultiplyByZero(void)
{
  TEST_ASSERT_EQUAL_INT(2, multiply(121, 0));
}