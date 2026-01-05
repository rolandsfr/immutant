/**
 * Environment management
 *
 * This environment implementation is based on linked list for simplicity, but
 * this can create performance bottlenecks for large scopes. For better
 * performance, consider using a hash table for EnvEntry storage.
 * */

#include "env.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ast_stmt.h"
#include "env.h"
#include "value_t.h"

Env* env_new(Env* parent, EnvPurity purity)
{
	Env* env = malloc(sizeof(Env));
	env->parent = parent;
	env->purity = purity;
	env->entries = NULL;
	return env;
}

void env_define_var(Env* env, const char* name, Value value,
					enum MutabilityType mutability)
{
	EnvEntry* entry = malloc(sizeof(EnvEntry));
	entry->name = strdup(name);
	entry->value = value;
	entry->value.mutability = mutability;
	entry->next = env->entries;
	env->entries = entry;
}

// TODO: remove alias after backwards compatibility is no longer needed
void env_define(Env* env, const char* name, Value value,
				enum MutabilityType mutability)
{
	env_define_var(env, name, value, mutability);
}

void env_define_fn(Env* env, const char* name, Value value,
				   enum PurityType purity)
{
	EnvEntry* entry = malloc(sizeof(EnvEntry));
	entry->name = strdup(name);
	entry->value = value;
	entry->value.purity = purity;
	entry->value.mutability = IMMUTABLE; // functions are immutable
	entry->next = env->entries;
	env->entries = entry;
}

Value* env_get(Env* env, const char* name)
{

	int crossed_function_boundary = 0;

	for (Env* e = env; e != NULL; e = e->parent) {

		if (e != env) {
			crossed_function_boundary++;
		}

		for (EnvEntry* entry = e->entries; entry != NULL; entry = entry->next) {
			if (strcmp(entry->name, name) == 0) {
				// print for variable purity textually

				if (env->purity == ENV_PURE &&
					entry->value.mutability == MUTABLE &&
					crossed_function_boundary > 1) {
					return NULL;
				}

				return &entry->value;
			}
		}
	}

	return NULL;
}

EnvEntry* env_get_direct_entry(Env* env, const char* name)
{
	for (EnvEntry* entry = env->entries; entry != NULL; entry = entry->next) {
		if (strcmp(entry->name, name) == 0) {
			return entry;
		}
	}
	return NULL; // not found
}

EnvEntry env_get_entry(Env* env, const char* name)
{
	int is_pure_env = (env->purity == ENV_PURE);
	for (Env* e = env; e != NULL; e = e->parent) {

		is_pure_env = (env->purity == ENV_PURE);
		for (EnvEntry* entry = e->entries; entry != NULL; entry = entry->next) {
			if (strcmp(entry->name, name) == 0) {

				if (entry->value.mutability == MUTABLE && is_pure_env) {
					return (EnvEntry){NULL};
				}

				return *entry;
			}
		}
	}

	return (EnvEntry){NULL};
}

int env_set(Env* env, const char* name, Value value)
{
	for (Env* e = env; e != NULL; e = e->parent) {
		for (EnvEntry* entry = e->entries; entry != NULL; entry = entry->next) {
			if (strcmp(entry->name, name) == 0) {
				entry->value = value;
				return 1; // success
			}
		}
	}
	return 0; // not found
}

void env_free(Env* env)
{
	if (!env)
		return;

	EnvEntry* entry = env->entries;
	while (entry) {
		EnvEntry* next = entry->next;
		free(entry->name);
		free(entry);
		entry = next;
	}

	free(env);
}
