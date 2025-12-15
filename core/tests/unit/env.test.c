#include "env.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "unity.h"

#include "value_t.h"

/* --- Test Values --- */
static Value v1;
static Value v2;
static Value v3;

/* --- Helpers --- */
void assert_value_equal(Value* a, Value* b)
{
	TEST_ASSERT_EQUAL(a->type, b->type);
	switch (a->type) {
		case VAL_NUMBER:
			TEST_ASSERT_EQUAL_DOUBLE(a->number, b->number);
			break;
		case VAL_BOOL:
			TEST_ASSERT_EQUAL(a->boolean, b->boolean);
			break;
		case VAL_STRING:
			TEST_ASSERT_EQUAL_STRING(a->string, b->string);
			break;
		default:
			TEST_FAIL_MESSAGE("Unknown Value type");
	}
}

/* --- Setup / Teardown --- */
void setUp(void)
{
	// Initialize some test values
	v1.type = VAL_NUMBER;
	v1.number = 42;
	v2.type = VAL_NUMBER;
	v2.number = -13;
	v3.type = VAL_NUMBER;
	v3.number = 1000;
}

void tearDown(void) { /* nothing to tear down */ }

/* --- Tests --- */

void test_env_define_and_get(void)
{
	Env* env = env_new(NULL);
	env_define(env, "a", &v1, MUTABLE);

	Value* result = env_get(env, "a");
	TEST_ASSERT_NOT_NULL(result);
	assert_value_equal(result, &v1);

	env_free(env);
}

void test_env_get_undefined_returns_null(void)
{
	Env* env = env_new(NULL);

	Value* result = env_get(env, "missing");
	TEST_ASSERT_NULL(result);

	env_free(env);
}

void test_env_set_updates_existing_variable(void)
{
	Env* env = env_new(NULL);

	env_define(env, "x", &v1, MUTABLE);
	int ok = env_set(env, "x", &v2);
	TEST_ASSERT_TRUE(ok);

	Value* updated = env_get(env, "x");
	TEST_ASSERT_NOT_NULL(updated);
	assert_value_equal(updated, &v2);

	env_free(env);
}

void test_env_set_undefined_variable_fails(void)
{
	Env* env = env_new(NULL);

	int ok = env_set(env, "x", &v1);
	TEST_ASSERT_FALSE(ok);

	env_free(env);
}

void test_env_lookup_parent_scope(void)
{
	Env* global = env_new(NULL);
	Env* local = env_new(global);

	env_define(global, "g", &v1, MUTABLE);

	Value* result = env_get(local, "g");
	TEST_ASSERT_NOT_NULL(result);
	assert_value_equal(result, &v1);

	env_free(local);
	env_free(global);
}

void test_env_shadowing(void)
{
	Env* global = env_new(NULL);
	Env* local = env_new(global);

	env_define(global, "x", &v1, MUTABLE);
	env_define(local, "x", &v2, MUTABLE);

	Value* result_local = env_get(local, "x");
	Value* result_global = env_get(global, "x");

	TEST_ASSERT_NOT_NULL(result_local);
	TEST_ASSERT_NOT_NULL(result_global);

	assert_value_equal(result_local, &v2);
	assert_value_equal(result_global, &v1);

	env_free(local);
	env_free(global);
}

void test_env_set_updates_nearest_scope(void)
{
	Env* global = env_new(NULL);
	Env* local = env_new(global);

	env_define(global, "x", &v1, MUTABLE);
	env_define(local, "y", &v2, MUTABLE);

	int ok = env_set(local, "x", &v3);
	TEST_ASSERT_TRUE(ok);

	Value* result_global = env_get(global, "x");
	TEST_ASSERT_NOT_NULL(result_global);
	assert_value_equal(result_global, &v3);

	env_free(local);
	env_free(global);
}
