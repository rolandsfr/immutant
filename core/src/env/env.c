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

Env* env_new(Env* parent)
{
	Env* env = malloc(sizeof(Env));
	env->parent = parent;
	env->entries = NULL;
	return env;
}

void env_define(Env* env, const char* name, Value value,
				enum MutabilityType mutability)
{
	EnvEntry* entry = malloc(sizeof(EnvEntry));
	entry->name = strdup(name);
	entry->value = value; // store directly
	entry->mutability = mutability;
	entry->next = env->entries;
	env->entries = entry;
}

Value* env_get(Env* env, const char* name)
{
	for (Env* e = env; e != NULL; e = e->parent) {
		for (EnvEntry* entry = e->entries; entry != NULL; entry = entry->next) {
			if (strcmp(entry->name, name) == 0) {
				return &entry->value;
			}
		}
	}
	return NULL; // not found
}

EnvEntry env_get_entry(Env* env, const char* name)
{
    for (Env* e = env; e != NULL; e = e->parent) {
        for (EnvEntry* entry = e->entries; entry != NULL; entry = entry->next) {
            if (strcmp(entry->name, name) == 0) {
                return *entry;
            }
        }
    }

    return (EnvEntry){
        NULL
    }; 
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
