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
	Env* env = env_new(NULL, ENV_IMPURE);
	env_define(env, "a", v1, MUTABLE); // pass by value

	Value* result = env_get(env, "a");
	TEST_ASSERT_NOT_NULL(result);
	assert_value_equal(result, &v1);

	env_free(env);
}

void test_env_get_undefined_returns_null(void)
{
	Env* env = env_new(NULL, ENV_IMPURE);

	Value* result = env_get(env, "missing");
	TEST_ASSERT_NULL(result);

	env_free(env);
}

void test_env_set_updates_existing_variable(void)
{
	Env* env = env_new(NULL, ENV_IMPURE);

	env_define(env, "x", v1, MUTABLE);
	int ok = env_set(env, "x", v2); // pass by value
	TEST_ASSERT_TRUE(ok);

	Value* updated = env_get(env, "x");
	TEST_ASSERT_NOT_NULL(updated);
	assert_value_equal(updated, &v2);

	env_free(env);
}

void test_env_set_undefined_variable_fails(void)
{
	Env* env = env_new(NULL, ENV_IMPURE);

	int ok = env_set(env, "x", v1); // pass by value
	TEST_ASSERT_FALSE(ok);

	env_free(env);
}

void test_env_lookup_parent_scope(void)
{
	Env* global = env_new(NULL, ENV_IMPURE);
	Env* local = env_new(global, ENV_IMPURE);

	env_define(global, "g", v1, MUTABLE); // pass by value

	Value* result = env_get(local, "g");
	TEST_ASSERT_NOT_NULL(result);
	assert_value_equal(result, &v1);

	env_free(local);
	env_free(global);
}

void test_env_shadowing(void)
{
	Env* global = env_new(NULL, ENV_IMPURE);
	Env* local = env_new(global, ENV_IMPURE);

	env_define(global, "x", v1, MUTABLE);
	env_define(local, "x", v2, MUTABLE);

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
	Env* global = env_new(NULL, ENV_IMPURE);
	Env* local = env_new(global, ENV_IMPURE);

	env_define(global, "x", v1, MUTABLE);
	env_define(local, "y", v2, MUTABLE);

	int ok = env_set(local, "x", v3); // pass by value
	TEST_ASSERT_TRUE(ok);

	Value* result_global = env_get(global, "x");
	TEST_ASSERT_NOT_NULL(result_global);
	assert_value_equal(result_global, &v3);

	env_free(local);
	env_free(global);
}

void test_env_set_does_not_create_new_variable(void)
{
	Env* global = env_new(NULL, ENV_IMPURE);
	Env* local = env_new(global, ENV_IMPURE);

	env_define(local, "y", v2, MUTABLE);

	int var_is_set = env_set(local, "x", v1);
	TEST_ASSERT_FALSE(var_is_set);

	Value* result_global = env_get(global, "x");
	Value* result_local = env_get(local, "x");

	TEST_ASSERT_NULL(result_global);
	TEST_ASSERT_NULL(result_local);

	env_free(local);
	env_free(global);
}

void test_env_get_entry_does_not_resolve_mutable_in_pure_env(void)
{
	Env* global = env_new(NULL, ENV_PURITY_UNSET);
	Env* pure_env = env_new(global, ENV_PURE);

	env_define(global, "a", v1, MUTABLE);
	env_define(global, "b", v2, IMMUTABLE);

	EnvEntry entry_a = env_get_entry(pure_env, "a");
	EnvEntry entry_b = env_get_entry(pure_env, "b");

	TEST_ASSERT_NULL(entry_a.name);
	TEST_ASSERT_NOT_NULL(entry_b.name);

	env_free(pure_env);
	env_free(global);
}

void test_get_direct_entry_returns_only_current_scope(void)
{
	Env* global = env_new(NULL, ENV_IMPURE);
	Env* local = env_new(global, ENV_IMPURE);

	env_define(global, "x", v1, MUTABLE);
	env_define(local, "y", v2, MUTABLE);

	EnvEntry* entry_x = env_get_direct_entry(local, "x");
	EnvEntry* entry_y = env_get_direct_entry(local, "y");

	TEST_ASSERT_NULL(entry_x);
	TEST_ASSERT_NOT_NULL(entry_y);

	env_free(local);
	env_free(global);
}
